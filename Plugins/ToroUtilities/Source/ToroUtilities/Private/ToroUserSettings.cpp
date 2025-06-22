// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "ToroUtilities.h"
#include "ToroConfigs.h"

UToroUserSettings::UToroUserSettings()
{
	UToroUserSettings::SetToDefaults();
}

void UToroUserSettings::CheckSupportedResolutions()
{
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	FullscreenRes = SupportedResolutions.Last();
	Algo::Reverse(SupportedResolutions);
}

void UToroUserSettings::CheckSupportedFidelityModes()
{
	SupportedFidelityModes = {
		EImageFidelityMode::None,
		EImageFidelityMode::FXAA,
		EImageFidelityMode::TAA,
		EImageFidelityMode::TSR,
		// EImageFidelityMode::FSR // TODO - Not updated
	};

	if (XeSS::IsXeSSSupported()) SupportedFidelityModes.Add(EImageFidelityMode::XeSS);
}

void UToroUserSettings::AutoConfigureQuality()
{
	const float Res = ScalabilityQuality.ResolutionQuality;
	RunHardwareBenchmark();
	SetResolutionScaleValueEx(Res);
	ApplyHardwareBenchmarkResults();
	CacheScalabilityDefaults();
}

void UToroUserSettings::SetOverallQuality(const uint8 InValue)
{
	if (InValue > 0)
	{
		SetViewDistanceQuality(InValue - 1);
		SetAntiAliasingQuality(InValue - 1);
		SetShadowQuality(InValue - 1);
		SetGlobalIlluminationQuality(InValue - 1);
		SetReflectionQuality(InValue - 1);
		SetPostProcessingQuality(InValue - 1);
		SetTextureQuality(InValue - 1);
		SetVisualEffectQuality(InValue - 1);
		SetFoliageQuality(InValue - 1);
		SetShadingQuality(InValue - 1);
	}
}

uint8 UToroUserSettings::GetOverallQuality() const
{
	const int32 Target = GetViewDistanceQuality();
	if (Target == GetAntiAliasingQuality()
		&& Target == GetShadowQuality()
		&& Target == GetGlobalIlluminationQuality()
		&& Target == GetReflectionQuality()
		&& Target == GetPostProcessingQuality()
		&& Target == GetTextureQuality()
		&& Target == GetVisualEffectQuality()
		&& Target == GetFoliageQuality()
		&& Target == GetShadingQuality())
	{
		return Target + 1;
	}

	return 0;
}

void UToroUserSettings::SetMaxFrameRateIdx(const uint8 InIndex)
{
	float Limit;
	switch (InIndex)
	{
	case 0: Limit = 30.0f; break;
	case 1: Limit = 60.0f; break;
	case 2: Limit = 120.0f; break;
	case 3: Limit = 500.0f; break;
	default: Limit = 60.0f; break;
	}
	SetFrameRateLimit(Limit);
}

uint8 UToroUserSettings::GetMaxFrameRateIdx() const
{
	float Limit = GetFrameRateLimit();
	if (Limit > 121.0f) return 3;
	if (Limit > 61.0f) return 2;
	if (Limit > 31.0f) return 1;
	return 0;
}

void UToroUserSettings::SetAudioVolume(const ESoundClassType InType, const uint8 InVolume)
{
	if (InType == ESoundClassType::MAX) return;
	AudioVolume.FindOrAdd(InType) = FMath::Clamp(InVolume, 25, 150);
	ApplyAudioSettings();
}

DEFINE_SETTER_DYNAMIC(bool, ShowFPS)
DEFINE_SETTER_BASIC(bool, SmoothCamera)
DEFINE_SETTER_BASIC(float, SensitivityX)
DEFINE_SETTER_BASIC(float, SensitivityY)

DEFINE_SETTER(EColorBlindMode, ColorBlindMode, ApplyColorBlindSettings();)
DEFINE_SETTER(uint8, ColorBlindIntensity, ApplyColorBlindSettings();)

DEFINE_SETTER_BASIC(bool, Borderless)

DEFINE_SETTER(float, Gamma, if (GEngine) GEngine->DisplayGamma = GetGamma();)
DEFINE_SETTER_DYNAMIC(uint8, Brightness)
DEFINE_SETTER_DYNAMIC(bool, FancyBloom)
DEFINE_SETTER_CONSOLE(bool, ScreenSpaceFogScattering, r.SSFS)
DEFINE_SETTER(uint8, MotionBlur, ApplyMotionBlur();)

DEFINE_SETTER(uint8, LumenGI, ApplyLumen();)
DEFINE_SETTER(uint8, LumenReflection, ApplyLumen();)
DEFINE_SETTER(bool, HitLightingReflections, ApplyLumen();)

DEFINE_SETTER(EImageFidelityMode, ImageFidelityMode, ApplyImageFidelityMode();)
DEFINE_SETTER_CONSOLE(uint8, TSRResolution, r.ScreenPercentage)

DEFINE_SETTER(uint8, FSRQuality, ApplyFSR();)
DEFINE_SETTER(float, FSRSharpness, ApplyFSR();)
DEFINE_SETTER_CONSOLE(bool, FSRFrameGeneration, r.FidelityFX.FI.Enabled)

DEFINE_SETTER(uint8, XeSSQuality,
	XeSS::SetXeSSMode(ImageFidelityMode == EImageFidelityMode::XeSS ? GetXeSSQuality() : 0);
)

void UToroUserSettings::InitSettings(UGameInstance* InGameInstance)
{
	GameInstance = InGameInstance;
	const UToroConfigManager* Manager = UToroConfigManager::Get(this);
	if (Manager && Manager->IsFirstLaunch())
	{
		RunHardwareBenchmark();
		ApplyHardwareBenchmarkResults();
		if (GetOverallQuality() > 3)
		{
			SetOverallQuality(3);
		}
		
		SetResolutionScaleNormalized(1.0f);
		CacheScalabilityDefaults();

		ReapplySettings();
	}
	else
	{
		LoadSettings(true);
	}
}

void UToroUserSettings::ApplyFSR() const
{
	SET_CONSOLE_VAR(r.FidelityFX.FSR3.QualityMode, GetFSRQuality())
	SET_CONSOLE_VAR(r.FidelityFX.FSR3.Sharpness, GetFSRSharpness())
}

void UToroUserSettings::ApplyImageFidelityMode()
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	if (SupportedFidelityModes.Contains(ImageFidelityMode))
	{
		uint8 AAMode = 0;
		bool bFSR = false, bXeSS = false;
		switch (ImageFidelityMode)
		{
		case EImageFidelityMode::None: break;
		case EImageFidelityMode::FXAA:
			AAMode = 1;
			break;
		case EImageFidelityMode::TAA:
			AAMode = 2;
			break;
		case EImageFidelityMode::TSR:
			AAMode = 4;
			break;
		case EImageFidelityMode::FSR:
			bFSR = true;
			ApplyFSR();
			break;
		case EImageFidelityMode::XeSS:
			bXeSS = true;
			break;
		}

		SET_CONSOLE_VAR(r.AntiAliasingMethod, AAMode)
		SET_CONSOLE_VAR(r.ScreenPercentage, AAMode == 4 ? GetTSRResolution() : 100.0f);

		SET_CONSOLE_VAR(r.FidelityFX.FSR3.Enable, bFSR)
		SET_CONSOLE_VAR(r.FidelityFX.FI.Enabled, bFSR && FSRFrameGeneration)

		XeSS::SetXeSSMode(bXeSS ? GetXeSSQuality() : 0);

		RefreshUI.Broadcast(this);
	}
	else
	{
		// Should never hit this fallback unless someone edits the file
		SetImageFidelityMode(EImageFidelityMode::TAA);
	}
}

void UToroUserSettings::ApplyColorBlindSettings() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(static_cast<EColorVisionDeficiency>(ColorBlindMode),
		GetColorBlindIntensity(), true, false);
}

void UToroUserSettings::ApplyAudioSettings() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UToroUtilSettings* Settings = UToroUtilSettings::Get();
	if (USoundMix* SoundMix = Settings->MainSoundMix.LoadSynchronous())
	{
		for (const ESoundClassType Type : TEnumRange<ESoundClassType>())
		{
			if (USoundClass* SoundClass = Settings->SoundClasses[static_cast<uint8>(Type)].LoadSynchronous())
			{
				UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, SoundClass,
					GetAudioVolume(Type), 1.0f, 0.5f, true);
			}
		}
	}
}

void UToroUserSettings::ApplyMotionBlur() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const bool bMotionBlur = GetMotionBlur() != 0;
	SET_CONSOLE_VAR(r.DefaultFeature.MotionBlur, bMotionBlur ? 1 : 0);
	if (bMotionBlur) OnDynamicSettingsChanged.Broadcast(this);
}

void UToroUserSettings::ApplyLumen() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const bool bLumenGI = GetLumenGI() != 0;
	SET_CONSOLE_VAR(r.DynamicGlobalIlluminationMethod,	bLumenGI ? 1 : 0);
	
	const bool bLumenReflection = GetLumenReflection() != 0;
	SET_CONSOLE_VAR(r.ReflectionMethod,	bLumenReflection ? 1 : 2);
	
	const bool bHitLighting = GetHitLightingReflections();
	SET_CONSOLE_VAR(r.Lumen.HardwareRayTracing.LightingMode, bHitLighting ? 2 : 0);
	
	if (bLumenGI || bLumenReflection || bHitLighting) OnDynamicSettingsChanged.Broadcast(this);
}

void UToroUserSettings::ReapplySettings()
{
#if WITH_EDITOR
	if (FApp::IsGame())
#endif
	{
		if (GEngine) GEngine->DisplayGamma = GetGamma();
		SET_CONSOLE_VAR(r.SSFS, ScreenSpaceFogScattering);
		OnDynamicSettingsChanged.Broadcast(this);

		ApplyImageFidelityMode();
		ApplyColorBlindSettings();
		ApplyAudioSettings();
		ApplyMotionBlur();
		ApplyLumen();

		ApplyNonResolutionSettings();
	}
}

void UToroUserSettings::CacheScalabilityDefaults()
{
	ScalabilityDefaults[0]	= GetOverallQuality();
	ScalabilityDefaults[1]	= GetViewDistanceQuality();
	ScalabilityDefaults[2]	= GetAntiAliasingQuality();
	ScalabilityDefaults[3]	= GetShadowQuality();
	ScalabilityDefaults[4]	= GetGlobalIlluminationQuality();
	ScalabilityDefaults[5]	= GetReflectionQuality();
	ScalabilityDefaults[6]	= GetPostProcessingQuality();
	ScalabilityDefaults[7]	= GetTextureQuality();
	ScalabilityDefaults[8]	= GetVisualEffectQuality();
	ScalabilityDefaults[9]	= GetFoliageQuality();
	ScalabilityDefaults[10] = GetShadingQuality();
}

void UToroUserSettings::SetToDefaults()
{
	ShowFPS = false;
	SmoothCamera = true;
	SensitivityX = 1.0f;
	SensitivityY = 1.0f;
	Gamma = 2.2f;
	Brightness = 100;
	FancyBloom = true;
	ScreenSpaceFogScattering = true;
	MotionBlur = 2;
	LumenGI = 0;
	LumenReflection = 2;
	HitLightingReflections = false;
	ColorBlindMode = EColorBlindMode::None;
	ColorBlindIntensity = 0;
	ImageFidelityMode = EImageFidelityMode::TAA;
	TSRResolution = 70;
	FSRQuality = 2;
	FSRSharpness = 0.0f;
	FSRFrameGeneration = false;
	XeSSQuality = 3;

	ReapplySettings();
	Super::SetToDefaults();
}

void UToroUserSettings::ApplyNonResolutionSettings()
{
	OnSettingsApplied.Broadcast(this);
	Super::ApplyNonResolutionSettings();
}

void UToroUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);
	ApplyResolutionSettings(true);
	ReapplySettings();
}

UWorld* UToroUserSettings::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GameInstance ? GameInstance->GetWorld() : nullptr;
	if (!World) World = GEngine->GetCurrentPlayWorld();
	return World;
}