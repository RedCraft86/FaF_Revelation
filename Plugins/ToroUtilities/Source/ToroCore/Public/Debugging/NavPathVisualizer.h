// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WireShapeComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "NavPathVisualizer.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType, meta = (HiddenCategories = "Actor,Collision"))
class TOROCORE_API ANavPathVisualizer final : public AActor
{
	GENERATED_BODY()

public:
	
	ANavPathVisualizer(): bRefresh(false), PathPoints({FVector{0.0f}})
	{
		PrimaryActorTick.bCanEverTick = false;

		SceneRoot = CreateDefaultSubobject<UBillboardComponent>("DefaultIconBillboard");
		SceneRoot->SetSprite(LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/EmptyActor.EmptyActor")));
		SceneRoot->bIsScreenSizeScaled = true;
		SceneRoot->SetHiddenInGame(true);
		SetRootComponent(SceneRoot);
	
#if WITH_EDITORONLY_DATA
		ShapeComponent = CreateEditorOnlyDefaultSubobject<UWireShapeComponent>(TEXT("ShapeComponent"));
		if (ShapeComponent)
		{
			FWireStringData Data; Data.String = TEXT("NavPath Visualizer");
			ShapeComponent->WireStrings.Add(TEXT("Label"), Data);
		}

		bRunConstructionScriptOnDrag = true;
#endif
		bIsEditorOnlyActor = true;
		SetCanBeDamaged(false);
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bRefresh;
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (MakeEditWidget = true))
		TArray<FVector> PathPoints;
	
private:
	
	UPROPERTY() TObjectPtr<UBillboardComponent> SceneRoot;
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UWireShapeComponent> ShapeComponent;
#endif
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		K2_DestroyActor();
	}
	
	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
		SetActorScale3D(FVector{1.0f});
#if WITH_EDITORONLY_DATA
		if (ShapeComponent)
		{
			ShapeComponent->WireNavPath.Targets = PathPoints;
			ShapeComponent->UpdateNavPoints();
		}
#endif
		bRefresh = false;
	}
};