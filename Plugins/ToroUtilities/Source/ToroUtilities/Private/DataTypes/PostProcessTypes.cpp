// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PostProcessTypes.h"

void FPPBloomOption::ModifyPP(FPostProcessSettings& Settings) const
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

void FPPBloomChoice::ApplyChoice(FPostProcessSettings& Settings, const bool bFancy) const
{
#if WITH_EDITORONLY_DATA
	if (!FApp::IsGame())
	{
		GetOption(bPreviewFancy).ModifyPP(Settings);
		return;
	}
#endif
	GetOption(bFancy).ModifyPP(Settings);
}

void FPPLumenOption::ModifyGI(FPostProcessSettings& Settings) const
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

void FPPLumenOption::ModifyReflection(FPostProcessSettings& Settings) const
{
	Settings.bOverride_LumenReflectionQuality = true;
	Settings.LumenReflectionQuality = ReflectionQuality;
		
	Settings.bOverride_LumenMaxReflectionBounces = true;
	Settings.LumenMaxReflectionBounces = ReflectionBounces;
		
	Settings.bOverride_LumenMaxRefractionBounces = true;
	Settings.LumenMaxRefractionBounces = RefractionBounces;
}

void FPPLumenChoice::ApplyChoice(FPostProcessSettings& Settings, const uint8 GIQuality, const uint8 ReflectionQuality) const
{
	Settings.bOverride_LumenRayLightingMode = true;
#if WITH_EDITORONLY_DATA
	if (!FApp::IsGame())
	{
		ApplyGI(Settings, GIQuality);
		ApplyReflection(Settings, ReflectionQuality);
		return;
	}
#endif
	ApplyGI(Settings, GIQuality);
	ApplyReflection(Settings, ReflectionQuality);
}

void FPPLumenChoice::ApplyGI(FPostProcessSettings& Settings, const uint8 Quality) const
{
	GetOption(Quality).ModifyGI(Settings);
}

void FPPLumenChoice::ApplyReflection(FPostProcessSettings& Settings, const uint8 Quality) const
{
	GetOption(Quality).ModifyReflection(Settings);
}

const FPPLumenOption& FPPLumenChoice::GetOption(const uint8 Quality) const
{
	switch (FMath::Max(0, Quality - 1))
	{
	case 0:	return Low;
	case 1:	return Medium;
	default:return High;
	}
}

void FPPMotionBlurOption::ModifyPP(FPostProcessSettings& Settings) const
{
	Settings.bOverride_MotionBlurAmount = true;
	Settings.bOverride_MotionBlurMax = true;

	Settings.MotionBlurAmount = Amount;
	Settings.MotionBlurMax = MaxDistortion;
}

void FPPMotionBlurChoice::ApplyChoice(FPostProcessSettings& Settings, const uint8 Quality) const
{
	Settings.bOverride_MotionBlurAmount = true;
#if WITH_EDITORONLY_DATA
	if (!FApp::IsGame())
	{
		if (PreviewQuality == 0)
		{
			Settings.MotionBlurAmount = 0.0f;
			Settings.MotionBlurMax = 0.0f;
		}
		else GetOption(Quality).ModifyPP(Settings);
		return;
	}
#endif
	if (Quality == 0)
	{
		Settings.MotionBlurAmount = 0.0f;
		Settings.MotionBlurMax = 0.0f;
	}
	else GetOption(Quality).ModifyPP(Settings);
}

const FPPMotionBlurOption& FPPMotionBlurChoice::GetOption(const uint8 Quality) const
{
	switch (FMath::Max(0, Quality - 1))
	{
	case 0:	return Low;
	case 1:	return Medium;
	default:return High;
	}
}
