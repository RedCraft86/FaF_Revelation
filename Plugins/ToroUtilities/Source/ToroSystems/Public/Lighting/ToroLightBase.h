// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroLightEntry.h"
#include "Actors/ToroActor.h"
#include "Components/ReferenceCulling.h"
#include "ToroLightBase.generated.h"

UCLASS(Abstract, meta = (ChildCanTick))
class TOROSYSTEMS_API AToroLightBase : public AToroActor
{
	GENERATED_BODY()

	friend class AToroLightSwitch;

public:

	AToroLightBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<URefCullComponent> RefCulling;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		bool bReferenceCull;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 FlickerType;

	UPROPERTY(EditAnywhere, Category = "Settings|Mesh", meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float Multiplier;

	UPROPERTY(EditAnywhere, Category = "Settings|Mesh")
		float Fresnel;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		TArray<UMaterialInterface*> FlickerMats;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Apply Light Settings")
		void ApplyLightSettings() const;

	/* Primitive Data for Meshes
	 * Idx 0..3	: RGB and Intensity
	 * Idx 4	: Fresnel
	 * Idx 5	: Flicker
	 */
	UFUNCTION(BlueprintImplementableEvent)
		TArray<FToroLightEntry> GetLights() const;

	UFUNCTION(BlueprintCallable, Category = ToroLight)
		void SetFlickerType(const uint8 InFlicker);

protected:

	void UpdateLights();
	virtual void EnableStateChanged(const bool bIsEnabled) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
