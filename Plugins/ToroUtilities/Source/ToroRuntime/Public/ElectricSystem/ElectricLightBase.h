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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightEntry)
		TObjectPtr<ULightComponent> Light;

	// Base intensity of the light.
	// This is what you would usually provide when setting the intensity from details panel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightEntry, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Intensity;

	// Key: Mesh Component | Value: Should the mesh turn invisible when off?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightEntry)
		TMap<TObjectPtr<UStaticMeshComponent>, bool> Meshes;

	FElectricLightEntry() : Intensity(0.0f) {}
};

USTRUCT(BlueprintInternalUseOnly)
struct FElectricLightAnim
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LightAnim)
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = LightAnim, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = LightAnim)
		FVector2D IntensityRange;

	UPROPERTY(EditAnywhere, Category = LightAnim)
		FInlineColorCurve AnimCurve;

	FElectricLightAnim();

	void Stop();
	void Play();
	void PlayLooping();
	void CacheValues();
	FLinearColor OnTick(const float DeltaTime);

	static void SetFlickerCurve(FElectricLightAnim& InAnim);
	
private:

	UPROPERTY() bool bPlaying;
	UPROPERTY() bool bLooping;
	UPROPERTY() float AnimTime;
	UPROPERTY() FVector2D TimeRange;
	UPROPERTY() FVector2D AlphaRange;
};

UCLASS(Abstract)
class TORORUNTIME_API AElectricLightBase final : public AElectricActorBase
{
	GENERATED_BODY()

public:

	AElectricLightBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UZoneCullingComponent> ZoneCulling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FElectricLightAnim LightUpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FElectricLightAnim FlickerAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FElectricLightAnim BreakAnim;
};
