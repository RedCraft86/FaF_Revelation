// Copyright (C) RedCraft86. All Rights Reserved.

#include "NavPathVisualizer.h"
#include "Components/BillboardComponent.h"

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

#if WITH_EDITORONLY_DATA
	bRunConstructionScriptOnDrag = true;
#endif
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

	bRefresh = false;
	SetActorScale3D(FVector{1.0f});
	if (ShapeComponent)
	{
		ShapeComponent->DebugNavPath.Targets = PathPoints;
		ShapeComponent->UpdateNavPoints();
	}
}

