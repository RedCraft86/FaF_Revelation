// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PostProcessTypes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FPPBloomOption
{
	GENERATED_BODY()

	/* Bloom algorithm */
	UPROPERTY(EditAnywhere, Category = Bloom)
		TEnumAsByte<EBloomMethod> Method;

	/* Multiplier for all bloom contributions >=0: off, 1(default), >1 brighter */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (ClampMin = 0.0f, UIMax = 8.0f))
		float Intensity;

	/* Minimum brightness the bloom starts having effect
	 * -1:all pixels affect bloom equally (physically correct, faster as a threshold pass is omitted), 0:all pixels affect bloom brights more, 1(default), >1 brighter
	 */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (ClampMin = -1.0f, UIMax = 8.0f, EditCondition = "Method == EBloomMethod::BM_SOG"))
		float Threshold;
	
	/* Intensity multiplier on the scatter dispersion energy of the kernel. 1.0 means exactly use the same energy as the kernel scatter dispersion. */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (ClampMin = 0.0f, UIMax = 20.0f, EditCondition = "Method == EBloomMethod::BM_FFT"))
		float ConvolutionScatterDispersion;

	/* Texture to replace default convolution bloom kernel */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (EditCondition = "Method == EBloomMethod::BM_FFT"))
		TObjectPtr<UTexture2D> ConvolutionKernel;

	FPPBloomOption(): Method(BM_SOG), Intensity(0.675f), Threshold(-1.0f)
		, ConvolutionScatterDispersion(1.0f), ConvolutionKernel(nullptr)
	{}

	void ModifyPP(FPostProcessSettings& Settings) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FPPBloomChoice
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Bloom)
		bool bPreviewFancy;
#endif

	UPROPERTY(EditAnywhere, Category = Bloom)
		FPPBloomOption Simple;

	UPROPERTY(EditAnywhere, Category = Bloom)
		FPPBloomOption Fancy;

	FPPBloomChoice(): bPreviewFancy(true) {}
	void ApplyChoice(FPostProcessSettings& Settings, const bool bFancy) const;

protected:

	const FPPBloomOption& GetOption(const bool bFancy) const { return bFancy ? Fancy : Simple; }
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FPPLumenOption
{
	GENERATED_BODY()

	/* Scales Lumen Scene's quality. Larger scales cause Lumen Scene to be calculated with a higher fidelity, which can be visible in reflections, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float SceneLightingQuality;

	/* Controls the size of instances that can be represented in Lumen Scene. Larger values will ensure small objects are represented, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.25f, UIMax = 4.0f))
		float SceneDetail;
	
	/* Controls how much Lumen Scene is allowed to cache lighting results to improve performance. Larger scales cause lighting changes to propagate faster, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.5f, UIMax = 4.0f))
		float SceneLightingUpdateSpeed;

	/* Scales Lumen's Final Gather quality. Larger scales reduce noise, but greatly increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float FinalGatherQuality;

	/* Controls how much Lumen Final Gather is allowed to cache lighting results to improve performance. Larger scales cause lighting changes to propagate faster, but increase GPU cost and noise. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.5f, UIMax = 4.0f))
		float FinalGatherLightingUpdateSpeed;

	/* Scales the Reflection quality. Larger scales reduce noise in reflections, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|Reflection", meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float ReflectionQuality;

	/* Sets the maximum number of recursive reflection bounces. 1 means a single reflection ray (no secondary reflections in mirrors). Currently only supported by Hardware Ray Tracing with Hit Lighting. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|Reflection", meta = (ClampMin = 1, ClampMax = 8))
		uint8 ReflectionBounces;

	/* The maximum count of refraction event to trace. When hit lighting is used, Translucent meshes will be traced when LumenMaxRefractionBounces > 0, making the reflection tracing more expensive. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|Reflection", meta = (ClampMin = 0, ClampMax = 64))
		uint8 RefractionBounces;

	FPPLumenOption(): SceneLightingQuality(1.0f), SceneDetail(1.0f), SceneLightingUpdateSpeed(1.0f), FinalGatherQuality(1.0f)
		, FinalGatherLightingUpdateSpeed(1.0f), ReflectionQuality(1.0f), ReflectionBounces(1), RefractionBounces(0)
	{}

	void ModifyGI(FPostProcessSettings& Settings) const;
	void ModifyReflection(FPostProcessSettings& Settings) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FPPLumenChoice
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Lumen, meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
		uint8 bPreviewGI;
	
	UPROPERTY(EditAnywhere, Category = Lumen, meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
		uint8 bPreviewReflection;
	
	UPROPERTY(EditAnywhere, Category = Lumen)
		bool bPreviewHitLighting;
#endif

	UPROPERTY(EditAnywhere, Category = Lumen)
		FPPLumenOption Low;

	UPROPERTY(EditAnywhere, Category = Lumen)
		FPPLumenOption Medium;

	UPROPERTY(EditAnywhere, Category = Lumen)
		FPPLumenOption High;

	FPPLumenChoice(): bPreviewGI(3), bPreviewReflection(3), bPreviewHitLighting(true) {}

	/* Quality 0~3 where 0: off and 3: high */
	void ApplyChoice(FPostProcessSettings& Settings, const uint8 GIQuality,
		const uint8 ReflectionQuality, const bool bHitLightingReflection) const;

protected:

	void ApplyGI(FPostProcessSettings& Settings, const uint8 Quality) const;
	void ApplyReflection(FPostProcessSettings& Settings, const uint8 Quality) const;
	const FPPLumenOption& GetOption(const uint8 Quality) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FPPMotionBlurOption
{
	GENERATED_BODY()

	/* Strength of motion blur, 0:off */
	UPROPERTY(Interp, EditAnywhere, Category = MotionBlur, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float Amount;

	/* Max distortion caused by motion blur, in percent of the screen width, 0:off */
	UPROPERTY(Interp, EditAnywhere, Category = MotionBlur, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float MaxDistortion;

	FPPMotionBlurOption(): Amount(0.5f), MaxDistortion(5.0f) {}
	void ModifyPP(FPostProcessSettings& Settings) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FPPMotionBlurChoice
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = MotionBlur, meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
		uint8 PreviewQuality;
#endif

	UPROPERTY(EditAnywhere, Category = MotionBlur)
		FPPMotionBlurOption Low;

	UPROPERTY(EditAnywhere, Category = MotionBlur)
		FPPMotionBlurOption Medium;

	UPROPERTY(EditAnywhere, Category = MotionBlur)
		FPPMotionBlurOption High;

	FPPMotionBlurChoice(): PreviewQuality(1) {}
	/* Quality 0~3 where 0: off and 3: high */
	void ApplyChoice(FPostProcessSettings& Settings, const uint8 Quality) const;

protected:

	const FPPMotionBlurOption& GetOption(const uint8 Quality) const;
};