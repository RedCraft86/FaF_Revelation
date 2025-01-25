// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbe.h"
#include "LightProbes/LightProbeManager.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ALightProbe::ALightProbe() : Intensity(1.0f), Radius(1000.0f), Falloff(2.0f), Color(FLinearColor::White)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	DebugShape = CreateEditorOnlyDefaultSubobject<UDebugShapeComponent>(TEXT("DebugShape"));
	DebugBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("DebugBillboard"));
	if (DebugBillboard)
	{
		DebugBillboard->SetSprite(LoadObject<UTexture2D>(nullptr,
			TEXT("Texture2D'/ToroUtilities/Icons/LightProbeIcon.LightProbeIcon'")));
	}
#endif
}

bool ALightProbe::IsRelevantProbe(const FTransform& Camera) const
{
	if (IsHidden()) return false;
	if (FMath::IsNearlyZero(Intensity) || Radius < 1.0f) return false;
	
	const FVector ThisLocation = GetActorLocation();
	FVector CameraLocation = Camera.GetTranslation();
	
	if (FVector::Dist(ThisLocation, CameraLocation) > 3000.0f) return false;

	CameraLocation -= Camera.GetRotation().Vector() * Radius;
	FVector ProbeToCam = ThisLocation - CameraLocation; ProbeToCam.Normalize();
	return FVector::DotProduct(Camera.GetRotation().Vector(), ProbeToCam) > 0.0f;
}

void ALightProbe::ApplyData(UMaterialParameterCollectionInstance* Collection, const uint8 Idx) const
{
	if (Collection)
	{
		Collection->SetScalarParameterValue(ParamName(Idx, false), Radius);
		Collection->SetVectorParameterValue(ParamName(Idx, true), {
			Color.R * Intensity,
			Color.G * Intensity,
			Color.B * Intensity,
			Falloff
		});
	}
}

// ReSharper disable once CppParameterMayBeConst
void ALightProbe::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	if (ULightProbeManager* Manager = ULightProbeManager::Get(this))
	{
		Manager->ForceProbeRecollection();
	}
}

#if WITH_EDITOR
void ALightProbe::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (DebugShape)
	{
		FDebugSphereData Data;
		Data.Radius = Radius;
		Data.Color = FColor::Turquoise;
		DebugShape->DebugSpheres.Add(TEXT("Radius"), Data);
	}
}
#endif
