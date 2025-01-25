// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "DebugShapeComponent.h"
#include "Components/BillboardComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "LightProbe.generated.h"

UCLASS()
class TORORUNTIME_API ALightProbe final : public AActor
{
	GENERATED_BODY()

public:

	ALightProbe();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UDebugShapeComponent> DebugShape;
	UPROPERTY() TObjectPtr<UBillboardComponent> DebugBillboard;
#endif

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f))
		float Intensity;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f))
		float Radius;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f))
		float Falloff;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (HideAlphaChannel))
		FLinearColor Color;
	
	bool IsRelevantProbe(const FTransform& Camera) const;
	void ApplyData(UMaterialParameterCollectionInstance* Collection, const uint8 Idx) const;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

#if WITH_EDITOR
private:
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
