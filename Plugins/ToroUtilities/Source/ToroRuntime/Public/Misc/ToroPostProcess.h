// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "DebugIconComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroPostProcess.generated.h"

USTRUCT(BlueprintType, DisplayName = "Bloom Settings")
struct FPPBloomSettings
{
	GENERATED_BODY()

	UPROPERTY(interp, EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Method"))
		TEnumAsByte<EBloomMethod> BloomMethod;
	
	UPROPERTY(interp, EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMax = 8.0f, DisplayName = "Intensity"))
		float BloomIntensity;

	UPROPERTY(interp, EditAnywhere, BlueprintReadWrite, Category=Settings, meta = (ClampMin = -1.0f, UIMax = 8.0f, DisplayName = "Threshold", EditCondition = "BloomMethod == EBloomMethod::BM_SOG"))
		float BloomThreshold;
	
	UPROPERTY(interp, EditAnywhere, BlueprintReadWrite, Category=Settings, meta = (ClampMin = 0.0f, UIMax = 20.0f, DisplayName = "Convolution Scatter Dispersion", EditCondition = "BloomMethod == EBloomMethod::BM_FFT"))
		float BloomConvolutionScatterDispersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Convolution Kernel", EditCondition = "BloomMethod == EBloomMethod::BM_FFT"))
		TObjectPtr<UTexture2D> BloomConvolutionTexture;

	FPPBloomSettings() : BloomMethod(BM_SOG), BloomIntensity(0.675f), BloomThreshold(-1.0f)
		, BloomConvolutionScatterDispersion(1.0f), BloomConvolutionTexture(nullptr)
	{}

	void AlterPostProcess(FPostProcessSettings& Settings) const
	{
		Settings.bOverride_BloomMethod = true;
		Settings.bOverride_BloomIntensity = true;
		Settings.bOverride_BloomThreshold = true;
		Settings.bOverride_BloomConvolutionScatterDispersion = true;
		Settings.bOverride_BloomConvolutionTexture = true;

		Settings.BloomMethod = BloomMethod;
		Settings.BloomIntensity = BloomIntensity;
		Settings.BloomThreshold = BloomThreshold;
		Settings.BloomConvolutionScatterDispersion = BloomConvolutionScatterDispersion;
		Settings.BloomConvolutionTexture = BloomConvolutionTexture;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPPBloomChoice
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings, Interp)
		bool bPreviewFancy = true;
#endif

	UPROPERTY(EditAnywhere, Category = Settings, Interp)
		FPPBloomSettings SimpleBloom;

	UPROPERTY(EditAnywhere, Category = Settings, Interp)
		FPPBloomSettings FancyBloom;

	FPPBloomChoice() {}
	
	void ApplyChoice(FPostProcessSettings& Settings, const bool bFancy) const;
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API AToroPostProcess final : public AActor
{
	GENERATED_BODY()

public:

	AToroPostProcess();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UBoxComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UPostProcessComponent> PostProcess;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UDebugIconComponent> DebugIcon;
#endif

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, Interp, meta = (DisplayPriority = -1))
		FPPBloomChoice Bloom;

	/* Post process settings to use for this volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, Interp, meta = (ShowOnlyInnerProperties))
		FPostProcessSettings Settings;

	/* Priority of this volume. In the case of overlapping volumes the one with the highest priority
	* overrides the lower priority ones. The order is undefined if two or more overlapping volumes have the same priority
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings)
		float Priority;

	/* World space radius around the volume that is used for blending (only if not unbound) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp, meta = (ClampMin = 0.0, UIMin = 0.0, UIMax = 6000.0f))
		float BlendRadius;

	/* 0: No effect, 1: Full effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp, meta = (UIMin = 0.0, UIMax = 1.0f))
		float BlendWeight;

	/* Whether this volume is enabled or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp)
		bool bEnabled;

	/* Set this false to use the parent shape component as volume bounds. True affects the whole world regardless */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = Settings, Interp)
		bool bUnbound;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Tools")
		bool bPreserveExposure = true;
	
	UPROPERTY(EditAnywhere, Category = "Tools")
		TSoftObjectPtr<const AActor> CopyTarget = nullptr;
	
	UFUNCTION(CallInEditor, Category = "Tools")
		void CopyFromTarget();
#endif
private:

	UPROPERTY(Transient) TObjectPtr<UToroUserSettings> UserSettings;

	void ApplySettings();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
