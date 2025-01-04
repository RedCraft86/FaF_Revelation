// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InlineCurves.h"
#include "ElectricActorBase.h"
#include "ZoneCullingComponent.h"
#include "ElectricLightBase.generated.h"

USTRUCT(BlueprintType)
struct FElectricLightEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightEntry")
		TObjectPtr<ULightComponent> Light;

	// Base intensity of the light.
	// This is what you would usually provide when setting the intensity from details panel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightEntry")
		float Intensity;

	// Key: Mesh Component | Value: Should the mesh turn invisible when off?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightEntry")
		TMap<TObjectPtr<UStaticMeshComponent>, bool> Meshes;

	FElectricLightEntry() : Intensity(0.0f) {}
};

UCLASS(Abstract)
class TORORUNTIME_API AElectricLightBase final : public AElectricActorBase
{
	GENERATED_BODY()
	
public:

	AElectricLightBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UZoneCullingComponent> ZoneCulling;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bPreviewState;

	UPROPERTY(EditAnywhere, Category = "Settings|Flicker")
		bool bFlicker;

	UPROPERTY(EditAnywhere, Category = "Settings|Flicker", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float FlickerRate;

	UPROPERTY(EditAnywhere, Category = "Settings|Flicker")
		FVector2D FlickerRange;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Settings|Flicker")
		float FlickerPlayback = 0.0f;
#endif
	
	UPROPERTY(EditAnywhere, Category = "Settings|Flicker")
		FInlineFloatCurve FlickerCurve;

	UPROPERTY(EditAnywhere, Category = "Settings|Mesh", DisplayName = "Multiplier")
		float MeshMulti;

	UPROPERTY(EditAnywhere, Category = "Settings|Mesh", DisplayName = "Fresnel")
		float MeshFresnel;

	UPROPERTY(EditAnywhere, Category = "Settings|Mesh", meta = (InlineEditConditionToggle))
		bool bMeshFlicker;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Mesh", DisplayName = "Flicker Range", meta = (EditCondition = "bMeshFlicker"))
		FVector2D MeshFlicker;

	UFUNCTION(BlueprintCallable, Category = "ElectricActor")
		void SetFlickerState(const bool bNewFlicker);

	UFUNCTION(BlueprintCallable, Category = "ElectricActor")
		void UpdateCaches();
	
	/**
	 * Primitive Data for Meshes
	 * Idx 0 -> 3	: Color and Base Intensity
	 * Idx 4		: Fresnel
	 * Idx 5		: Multiplier
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void GetLightInfo(TArray<FElectricLightEntry>& Entries) const;
	
protected:
	
	UPROPERTY() float FlickerTime;
	UPROPERTY() FVector2D FlickerValRange;
	UPROPERTY() FVector2D FlickerTimeRange;
	UPROPERTY(Transient) TArray<FElectricLightEntry> CachedEntries;

	virtual void OnStateChanged(const bool bState) override;
	virtual void OnEnableStateChanged(const bool bIsEnabled) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
#endif
};
