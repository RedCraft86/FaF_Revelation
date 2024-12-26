// Copyright (C) RedCraft86. All Rights Reserved.

#include "Debugging/NavPathVisualizer.h"
#include "Components/BillboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

ANavPathVisualizer::ANavPathVisualizer() : bRefresh(false), PathPoints({FVector{0.0f}})
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<UBillboardComponent>("DefaultIconBillboard");
	SceneRoot->SetSprite(LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/EmptyActor.EmptyActor")));
	SceneRoot->bIsScreenSizeScaled = true;
	SceneRoot->SetHiddenInGame(true);
	SetRootComponent(SceneRoot);
	
	ShapeComponent = CreateEditorOnlyDefaultSubobject<UDebugShapeComponent>(TEXT("ShapeComponent"));
	if (ShapeComponent)
	{
		FDebugStringData Data; Data.String = TEXT("NavPath Visualizer");
		ShapeComponent->DebugStrings.Add(TEXT("Label"), Data);
	}
	
	bIsEditorOnlyActor = true;
	SetCanBeDamaged(false);
}

void ANavPathVisualizer::BeginPlay()
{
	Super::BeginPlay();
	K2_DestroyActor();
}

void ANavPathVisualizer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector{1.0f});
	bRefresh = false;

	if (!ShapeComponent) return;
	ShapeComponent->DebugPoints.Empty(PathPoints.Num());
	ShapeComponent->DebugLines.Empty(PathPoints.Num() - 1);
	
	const int32 NumPoints = PathPoints.Num();
	if (NumPoints < 2) return;
	
	FDebugPointData PointData;
	PointData.bRenderOnTopOfEverything = true;
	PointData.Color = FLinearColor::Green;
	PointData.Size = 16.0f;

	FDebugLineData LineData;
	LineData.bRenderOnTopOfEverything = true;
	LineData.Color = FLinearColor::Yellow;
	LineData.Thickness = 4.0f;
	
	int idx = 0;
	const FTransform T = GetActorTransform();
	for (int i = 0; i < NumPoints - 1; i++)
	{
		if (const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
			T.TransformPositionNoScale(PathPoints[i]),
			T.TransformPositionNoScale(PathPoints[i + 1])))
		{
			const TArray<FVector> Points = NavPath->PathPoints;
			for (int j = 0; j < Points.Num(); j++)
			{
				PointData.Location = T.InverseTransformPositionNoScale(Points[j]);
				ShapeComponent->DebugPoints.Add(*FString::FromInt(idx), PointData);
				if (j < Points.Num() - 1)
				{
					LineData.Start = T.InverseTransformPositionNoScale(Points[j]);
					LineData.End = T.InverseTransformPositionNoScale(Points[j + 1]);
					ShapeComponent->DebugLines.Add(*FString::FromInt(idx), LineData);
				}
					
				idx++;
			}
		}
	}
}
