// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/DebugShapeComponent.h"
#include "NavPathVisualizer.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType, meta = (HiddenCategories = "Actor,Collision"))
class TOROCORE_API ANavPathVisualizer final : public AActor
{
	GENERATED_BODY()

public:
	
	ANavPathVisualizer();

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bRefresh;
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (MakeEditWidget = true))
		TArray<FVector> PathPoints;
	
private:
	
	UPROPERTY() TObjectPtr<UBillboardComponent> SceneRoot;
	UPROPERTY() TObjectPtr<UDebugShapeComponent> ShapeComponent;
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
