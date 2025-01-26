// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbe.h"
#include "LightProbes/LightProbeManager.h"

ALightProbe::ALightProbe() : Intensity(1.0f), Radius(500.0f), Falloff(2.0f)
	, Color(FLinearColor::White), MaxDistance(3000.0f), FadeRange(100.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	DebugShape = CreateEditorOnlyDefaultSubobject<UDebugShapeComponent>(TEXT("DebugShape"));
	DebugBillboard = CreateEditorOnlyDefaultSubobject<UMaterialBillboardComponent>(TEXT("DebugBillboard"));
	if (DebugBillboard)
	{
		DebugBillboard->SetupAttachment(SceneRoot);
		DebugBillboard->SetHiddenInGame(true);
	}
#endif
}

bool ALightProbe::IsRelevantProbe(const FTransform& Camera) const
{
	if (IsHidden()) return false;
	if (FMath::IsNearlyZero(Intensity) || Radius < 50.0f) return false;
	
	const FVector ThisLocation = GetActorLocation();
	FVector CameraLocation = Camera.GetTranslation();
	
	if (FVector::Dist(ThisLocation, CameraLocation) > MaxDistance) return false;

	CameraLocation -= Camera.GetRotation().Vector() * Radius;
	FVector ProbeToCam = ThisLocation - CameraLocation; ProbeToCam.Normalize();
	return FVector::DotProduct(Camera.GetRotation().Vector(), ProbeToCam) > 0.0f;
}

void ALightProbe::ApplyData(UMaterialInstanceDynamic* Material, const uint8 Idx) const
{
	if (Material)
	{
		Material->SetVectorParameterValue(ParamName(Idx, false), FLinearColor{
			Color.R * Intensity,
			Color.G * Intensity,
			Color.B * Intensity,
			Falloff
		});

		const FVector Pos = GetActorLocation();
		Material->SetVectorParameterValue(ParamName(Idx, true), FLinearColor{
			(float)Pos.X, (float)Pos.Y, (float)Pos.Z, Radius
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
		Data.NumOfSides = 64;
		Data.Radius = Radius;
		Data.Color = FColor::FromHex(TEXT("97C1D0FF"));
		DebugShape->DebugSpheres.Add(TEXT("Radius"), Data);
	}
	if (DebugBillboard)
	{
		if (!DebugMaterial)
		{
			if (UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr,
				TEXT("MaterialInterface'/ToroUtilities/Icons/LightProbe/M_LightProbe.M_LightProbe'")))
			{
				DebugMaterial = UMaterialInstanceDynamic::Create(BaseMat, DebugBillboard);
			}
			
			if (!DebugBillboard->Elements.IsEmpty()) DebugBillboard->SetElements({});
		}

		if (DebugMaterial)
		{
			DebugMaterial->SetVectorParameterValue(TEXT("Color"), Color);
			if (DebugBillboard->Elements.IsEmpty())
			{
				FMaterialSpriteElement Sprite;
				Sprite.Material = DebugMaterial;
				Sprite.BaseSizeX = Sprite.BaseSizeY = 40.0f;
				Sprite.DistanceToSizeCurve = LoadObject<UCurveFloat>(nullptr,
					TEXT("CurveFloat'/ToroUtilities/Icons/LightProbe/C_LightProbe.C_LightProbe'"));
				
				DebugBillboard->SetElements({Sprite});
			}
		}
	}
}
#endif
