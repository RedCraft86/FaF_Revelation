// Copyright (C) RedCraft86. All Rights Reserved.

#include "SplineBarrier.h"

ASplineBarrier::ASplineBarrier() : WallHeight(2.0f)
{
	SplineComponent->SetMobility(EComponentMobility::Static);
	SceneRoot->SetMobility(EComponentMobility::Static);

	WallMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("WallMeshComponent");
	WallMeshComponent->SetMobility(EComponentMobility::Static);
	WallMeshComponent->SetupAttachment(GetRootComponent());
	WallMeshComponent->SetLightingChannels(false, false, false);
	WallMeshComponent->bAffectDynamicIndirectLighting = false;
	WallMeshComponent->bAffectDistanceFieldLighting = false;
	WallMeshComponent->SetGenerateOverlapEvents(false);
	WallMeshComponent->SetCastShadow(false);

	SetHidden(true);

	bClosedLoop = true;
	SplineType = ESplinePointType::Linear;
	Collision.SetProfileName(TEXT("InvisibleWall"));
	
#if WITH_EDITOR
	LoadObjects();
#endif
}

#if WITH_EDITOR
void ASplineBarrier::LoadObjects()
{
	if (!Mesh) Mesh = LoadObject<UStaticMesh>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/SM_ProcBarrier.SM_ProcBarrier"));

	if (!Material) Material = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/M_ProcBarrier_01.M_ProcBarrier_01"));

	if (!OverlayMaterial) OverlayMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/M_ProcBarrier_02.M_ProcBarrier_02"));
}
#endif

void ASplineBarrier::BeginPlay()
{
	Super::BeginPlay();
	WallMeshComponent->SetOverlayMaterial(nullptr);
}

void ASplineBarrier::Construct()
{
#if WITH_EDITOR
	LoadObjects();
#endif

	const int GenPointCount = SplineComponent->GetNumberOfSplinePoints() - (bClosedLoop ? 0 : 1);
	
	WallMeshComponent->ClearInstances();
	WallMeshComponent->SetStaticMesh(Mesh);
	WallMeshComponent->SetMaterial(0, Material);
	UPrimitiveDataLibrary::SetPrimitiveCollision(WallMeshComponent, Collision);
	
	for (int32 i = 0; i < GenPointCount; i++)
	{
		if (SkipIndexes.Contains(i)) continue;
		
		FVector Location, Tangent;
		SplineComponent->GetLocalLocationAndTangentAtSplinePoint(i, Location, Tangent);
		const float DistB = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i + 1);
		const float DistA = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);

		WallMeshComponent->AddInstance({
			Tangent.Rotation(), Location,
			{(DistB - DistA) / 100.0f, 1.0f, WallHeight}
		});
	}
	
#if WITH_EDITORONLY_DATA
	WallMeshComponent->SetOverlayMaterial(OverlayMaterial);
#endif
	Super::Construct();
}
