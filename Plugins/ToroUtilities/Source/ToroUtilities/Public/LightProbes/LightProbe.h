﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "DebugShapeComponent.h"
#include "Components/MaterialBillboardComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "LightProbe.generated.h"

UCLASS()
class TOROUTILITIES_API ALightProbe final : public AActor
{
	GENERATED_BODY()

public:

	ALightProbe();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UDebugShapeComponent> DebugShape;
	UPROPERTY() TObjectPtr<UMaterialBillboardComponent> DebugBillboard;
	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> DebugMaterial;
#endif

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f))
		float Intensity;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 50.0f))
		float Radius;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f))
		float Falloff;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (HideAlphaChannel))
		FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bDisableWithLumen;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 1000.0f))
		float MaxDistance;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 0.0f))
		float FadeRange;
	
	bool IsRelevantProbe(const FTransform& Camera, const bool bHasLumen) const;
	void ApplyData(UMaterialInstanceDynamic* Material, const uint8 Idx, const FVector& Camera) const;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

#if WITH_EDITOR
private:
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
