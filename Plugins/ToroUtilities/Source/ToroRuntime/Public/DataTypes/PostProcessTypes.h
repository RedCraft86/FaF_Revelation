// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/ToroUserSettings.h"
#include "PostProcessTypes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FPPBloom
{
	GENERATED_BODY()

	/** Bloom algorithm */
	UPROPERTY(Interp, EditAnywhere, Category = Settings)
		TEnumAsByte<EBloomMethod> Method;

	/** Multiplier for all bloom contributions >=0: off, 1(default), >1 brighter */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMax = 8.0f))
		float Intensity;
	
	/**
	 * minimum brightness the bloom starts having effect
	 * -1:all pixels affect bloom equally (physically correct, faster as a threshold pass is omitted), 0:all pixels affect bloom brights more, 1(default), >1 brighter
	 */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = -1.0f, UIMax = 8.0f, EditCondition = "Method == EBloomMethod::BM_SOG"))
		float Threshold;
	
	/** Intensity multiplier on the scatter dispersion energy of the kernel. 1.0 means exactly use the same energy as the kernel scatter dispersion. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMax = 20.0f, EditCondition = "Method == EBloomMethod::BM_FFT"))
		float ConvolutionScatterDispersion;

	/** Texture to replace default convolution bloom kernel */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (EditCondition = "Method == EBloomMethod::BM_FFT"))
		TObjectPtr<UTexture2D> ConvolutionKernel;

	FPPBloom() : Method(BM_SOG), Intensity(0.675f), Threshold(-1.0f)
		, ConvolutionScatterDispersion(1.0f), ConvolutionKernel(nullptr)
	{}

	void ModifyPP(FPostProcessSettings& Settings) const
	{
		Settings.bOverride_BloomMethod = true;
		Settings.bOverride_BloomIntensity = true;
		Settings.bOverride_BloomThreshold = true;
		Settings.bOverride_BloomConvolutionScatterDispersion = true;
		Settings.bOverride_BloomConvolutionTexture = true;

		Settings.BloomMethod = Method;
		Settings.BloomIntensity = Intensity;
		Settings.BloomThreshold = Threshold;
		Settings.BloomConvolutionScatterDispersion = ConvolutionScatterDispersion;
		Settings.BloomConvolutionTexture = ConvolutionKernel;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPPLumen
{
	GENERATED_BODY()

	/** Scales Lumen Scene's quality. Larger scales cause Lumen Scene to be calculated with a higher fidelity, which can be visible in reflections, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float SceneLightingQuality;

	/** Controls the size of instances that can be represented in Lumen Scene. Larger values will ensure small objects are represented, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.25f, UIMax = 4.0f))
		float SceneDetail;
	
	/** Controls how much Lumen Scene is allowed to cache lighting results to improve performance. Larger scales cause lighting changes to propagate faster, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMax = 4.0f))
		float SceneLightingUpdateSpeed;

	/** Scales Lumen's Final Gather quality. Larger scales reduce noise, but greatly increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float FinalGatherQuality;

	/** Controls how much Lumen Final Gather is allowed to cache lighting results to improve performance. Larger scales cause lighting changes to propagate faster, but increase GPU cost and noise. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMax = 4.0f))
		float FinalGatherLightingUpdateSpeed;

	/** Scales the Reflection quality. Larger scales reduce noise in reflections, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float ReflectionQuality;
	
	FPPLumen() : SceneLightingQuality(1.0f), SceneDetail(1.0f), SceneLightingUpdateSpeed(1.0f)
		, FinalGatherQuality(1.0f), FinalGatherLightingUpdateSpeed(1.0f), ReflectionQuality(1.0f)
	{}

	void ModifyGI(FPostProcessSettings& Settings) const
	{
		Settings.bOverride_LumenSceneLightingQuality = true;
		Settings.bOverride_LumenSceneDetail = true;
		Settings.bOverride_LumenSceneLightingUpdateSpeed = true;
		Settings.bOverride_LumenFinalGatherQuality = true;
		Settings.bOverride_LumenFinalGatherLightingUpdateSpeed = true;

		Settings.LumenSceneLightingQuality = SceneLightingQuality;
		Settings.LumenSceneDetail = SceneDetail;
		Settings.LumenSceneLightingUpdateSpeed = SceneLightingUpdateSpeed;
		Settings.LumenFinalGatherQuality = FinalGatherQuality;
		Settings.LumenFinalGatherLightingUpdateSpeed = FinalGatherLightingUpdateSpeed;
	}

	void ModifyReflection(FPostProcessSettings& Settings) const
	{
		Settings.bOverride_LumenReflectionQuality = true;
		Settings.LumenReflectionQuality = ReflectionQuality;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPPMotionBlur
{
	GENERATED_BODY()

	/** Strength of motion blur, 0:off */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float Amount;

	/** Max distortion caused by motion blur, in percent of the screen width, 0:off */
	UPROPERTY(Interp, EditAnywhere, Category = Settings, meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float MaxDistortion;
	
	FPPMotionBlur() : Amount(0.5f), MaxDistortion(5.0f) {}

	void ModifyPP(FPostProcessSettings& Settings) const
	{
		Settings.bOverride_MotionBlurAmount = true;
		Settings.bOverride_MotionBlurMax = true;

		Settings.MotionBlurAmount = Amount;
		Settings.MotionBlurMax = MaxDistortion;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPPSettingOverrides
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bViewFancyBloom = true;

	// Global Illumination | Reflections | Motion Blur
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "View Qualities: GI | Reflect | Motion Blur", meta = (ClampMin = 0, UIMin = 0, ClampMax = 3, UIMax = 3))
		FIntVector ViewQualities = {1, 1, 1};
#endif

	UPROPERTY(Interp, EditAnywhere, Category = Settings)
		FPPBloom SimpleBloom;

	UPROPERTY(Interp, EditAnywhere, Category = Settings)
		FPPBloom FancyBloom;

	UPROPERTY(Interp, EditAnywhere, Category = Settings, EditFixedSize)
		TArray<FPPLumen> LumenQuality;

	UPROPERTY(Interp, EditAnywhere, Category = Settings, EditFixedSize)
		TArray<FPPMotionBlur> MotionBlur;
	
	FPPSettingOverrides() : LumenQuality({{}, {}, {}, {}})
		, MotionBlur({{}, {}, {}, {}})
	{}
	
	void ApplyChoice(FPostProcessSettings& PostProcess, const UToroUserSettings* User) const
	{
#if WITH_EDITORONLY_DATA
		if (!FApp::IsGame())
		{
			bViewFancyBloom ? FancyBloom.ModifyPP(PostProcess)
				: SimpleBloom.ModifyPP(PostProcess);
			
			LumenQuality[ViewQualities.X].ModifyGI(PostProcess);
			LumenQuality[ViewQualities.Y].ModifyReflection(PostProcess);

			MotionBlur[ViewQualities.Z].ModifyPP(PostProcess);
		}
		else
#endif
		{
			User->GetFancyBloom() ? FancyBloom.ModifyPP(PostProcess)
				: SimpleBloom.ModifyPP(PostProcess);
			
			LumenQuality[User->GetLumenGIQuality()].ModifyGI(PostProcess);
			LumenQuality[User->GetReflectionQuality()].ModifyReflection(PostProcess);

			MotionBlur[User->GetMotionBlurAmount()].ModifyPP(PostProcess);

			PostProcess.bOverride_DynamicGlobalIlluminationMethod = true;
			PostProcess.DynamicGlobalIlluminationMethod = User->GetLumenGI()
				? EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;

			PostProcess.bOverride_ReflectionMethod = true;
			PostProcess.ReflectionMethod = User->GetLumenGI()
				? EReflectionMethod::Lumen : EReflectionMethod::ScreenSpace;

			PostProcess.bOverride_LumenRayLightingMode = true;
			PostProcess.LumenRayLightingMode = User->GetHitLightingReflections() ?
				ELumenRayLightingModeOverride::HitLightingForReflections : ELumenRayLightingModeOverride::SurfaceCache;
		}
	}
};