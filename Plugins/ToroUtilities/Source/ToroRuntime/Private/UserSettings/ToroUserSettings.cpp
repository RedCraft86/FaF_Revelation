// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"
#include "UserSettings/UpscalerWrappers.h"
#include "ToroRuntimeSettings.h"
#include "AudioDevice.h"

UToroUserSettings::UToroUserSettings()
{
	SetToDefaults();
}

void UToroUserSettings::CheckSupportedFidelityModes()
{
	SupportedFidelityModes = {EImageFidelityMode::FXAA, EImageFidelityMode::TAA,
		EImageFidelityMode::TSR, EImageFidelityMode::SMAA, EImageFidelityMode::FSR};

	if (IsDLSSSupported()) SupportedFidelityModes.Add(EImageFidelityMode::DLSS);
	if (IsXeSSSupported()) SupportedFidelityModes.Add(EImageFidelityMode::XeSS);
}

bool UToroUserSettings::IsDynamicVibranceSupported()
{
	return Nvidia::Streamline::IsDeepDVCSupported();
}

bool UToroUserSettings::IsNvidiaReflexSupported()
{
	return Nvidia::Streamline::IsReflexSupported();
}

bool UToroUserSettings::IsDLSSFGSupported()
{
	return Nvidia::DLSS::IsDLSSGSupported();
}

bool UToroUserSettings::IsDLSSRRSupported()
{
	return Nvidia::DLSS::IsDLSSRRSupported();
}

bool UToroUserSettings::IsDLSSSupported()
{
	return Nvidia::DLSS::IsDLSSSupported();
}

bool UToroUserSettings::IsNISSupported()
{
	return Nvidia::NIS::IsNISSupported();
}

bool UToroUserSettings::IsXeSSSupported()
{
	return XeSS::IsXeSSSupported();
}

void UToroUserSettings::AutoConfigureQuality()
{
	const float Res = ScalabilityQuality.ResolutionQuality;
	RunHardwareBenchmark();
	SetResolutionScaleValueEx(Res);
	CacheScalabilityDefaults();
	ApplyHardwareBenchmarkResults();
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

void UToroUserSettings::SetAudioVolume(const ESoundClassType InType, const uint8 InVolume)
{
	if (InType == ESoundClassType::MAX) return;
	AudioVolume.FindOrAdd(InType) = FMath::Clamp(InVolume, 25, 150);
	ApplyAudioSettings();
}

DEFINE_SETTER_DYNAMIC(bool, ShowFPS)
DEFINE_SETTER_BASIC(FString, Username)

DEFINE_SETTER_DYNAMIC(uint8, FieldOfView)
DEFINE_SETTER_BASIC(bool, SmoothCamera)
DEFINE_SETTER_BASIC(float, SensitivityX)
DEFINE_SETTER_BASIC(float, SensitivityY)

DEFINE_SETTER(float, Gamma, if (GEngine) GEngine->DisplayGamma = GetGamma();)
DEFINE_SETTER_DYNAMIC(uint8, Brightness)
DEFINE_SETTER_DYNAMIC(bool, FancyBloom)
DEFINE_SETTER_CONSOLE(bool, ScreenSpaceFogScattering, r.SSFS)
DEFINE_SETTER(uint8, MotionBlur, ApplyMotionBlur();)

DEFINE_SETTER(uint8, LumenGI, ApplyLumen();)
DEFINE_SETTER(uint8, LumenReflection, ApplyLumen();)
DEFINE_SETTER(bool, HitLightingReflections, ApplyLumen();)

DEFINE_SETTER(EColorBlindMode, ColorBlindMode, ApplyColorBlindSettings();)
DEFINE_SETTER(uint8, ColorBlindIntensity, ApplyColorBlindSettings();)

DEFINE_SETTER(uint8, NvidiaReflex, Nvidia::Streamline::SetReflexMode(GetNvidiaReflex());)

DEFINE_SETTER(bool, RTXDynamicVibrance, ApplyDynamicVibrance();)
DEFINE_SETTER(float, DynamicVibranceIntensity, ApplyDynamicVibrance();)
DEFINE_SETTER(float, DynamicVibranceSaturation, ApplyDynamicVibrance();)
DEFINE_SETTER(uint8, NISQuality, ApplyNIS();)
DEFINE_SETTER(float, NISSharpness, ApplyNIS();)

DEFINE_SETTER(EImageFidelityMode, ImageFidelityMode, ApplyImageFidelityMode();)

DEFINE_SETTER(uint8, FXAADithering, SET_CONSOLE_VAR(r.FXAA.Quality, GetFXAADithering() + 1))
DEFINE_SETTER_CONSOLE(uint8, TAAUpsampling, r.TemporalAA.Quality)
DEFINE_SETTER_CONSOLE(uint8, TSRResolution, r.ScreenPercentage)

DEFINE_SETTER_CONSOLE(uint8, SMAAQuality, r.SMAA.Quality)
DEFINE_SETTER_CONSOLE(uint8, SMAAEdgeMode, r.SMAA.EdgeDetector)

DEFINE_SETTER(uint8, DLSSQuality, ApplyDLSS();)
DEFINE_SETTER(uint8, DLSSFrameGeneration, ApplyDLSS();)
DEFINE_SETTER(bool, DLSSRayReconstruction, ApplyDLSS();)

DEFINE_SETTER(uint8, FSRQuality, ApplyFSR();)
DEFINE_SETTER(float, FSRSharpness, ApplyFSR();)
DEFINE_SETTER_CONSOLE(bool, FSRFrameGeneration, r.FidelityFX.FI.Enabled)

DEFINE_SETTER(uint8, XeSSQuality,
	XeSS::SetXeSSMode(ImageFidelityMode == EImageFidelityMode::XeSS ? GetXeSSQuality() : 0);
)

void UToroUserSettings::ApplyNIS() const
{
	Nvidia::NIS::SetNISMode(GetNISQuality());
	Nvidia::NIS::SetNISSharpness(GetNISSharpness());
}

void UToroUserSettings::ApplyFSR() const
{
	SET_CONSOLE_VAR(r.FidelityFX.FSR3.QualityMode, GetFSRQuality())
	SET_CONSOLE_VAR(r.FidelityFX.FSR3.Sharpness, GetFSRSharpness())
}

void UToroUserSettings::ApplyDLSS() const
{
	const bool bDLSS = ImageFidelityMode == EImageFidelityMode::DLSS;
	Nvidia::DLSS::SetDLSSMode(bDLSS ? GetDLSSQuality() : 0);
	Nvidia::DLSS::SetFrameGenMode(bDLSS ? GetDLSSFrameGeneration() : 0);
	Nvidia::DLSS::SetDLSSRREnabled(bDLSS && GetDLSSRayReconstruction());
}

void UToroUserSettings::ApplyImageFidelityMode()
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	if (SupportedFidelityModes.Contains(ImageFidelityMode))
	{
		uint8 AAMode = 0;
		bool bSMAA = false, bFSR = false, bXeSS = false;
		switch (ImageFidelityMode)
		{
		case EImageFidelityMode::None: break;
		case EImageFidelityMode::FXAA:
			AAMode = 1;
			SET_CONSOLE_VAR(r.FXAA.Quality, GetFXAADithering() + 1)
			break;
		case EImageFidelityMode::TAA:
			AAMode = 2;
			SET_CONSOLE_VAR(r.TemporalAA.Quality, GetTAAUpsampling())
			break;
		case EImageFidelityMode::SMAA:
			bSMAA = true;
			SET_CONSOLE_VAR(r.SMAA.Quality, GetSMAAQuality())
			SET_CONSOLE_VAR(r.SMAA.EdgeDetector, GetSMAAEdgeMode())
			break;
		case EImageFidelityMode::TSR:
			AAMode = 4;
			SET_CONSOLE_VAR(r.ScreenPercentage, GetTSRResolution())
			break;
		case EImageFidelityMode::DLSS:
			ApplyDLSS();
			break;
		case EImageFidelityMode::FSR:
			bFSR = true;
			ApplyFSR();
			break;
		case EImageFidelityMode::XeSS:
			bXeSS = true;
			break;
		}

		SET_CONSOLE_VAR(r.SMAA, bSMAA)

		SET_CONSOLE_VAR(r.AntiAliasingMethod, AAMode)
		SET_CONSOLE_VAR(r.ScreenPercentage, AAMode == 4 ? TSRResolution : 100.0f);

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

void UToroUserSettings::ApplyDynamicVibrance() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	Nvidia::Streamline::SetDynamicVibrance(GetRTXDynamicVibrance(),
		GetDynamicVibranceIntensity(), GetDynamicVibranceSaturation());
}

void UToroUserSettings::ApplyAudioSettings() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
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
	const uint8 MotionBlurAmount = GetMotionBlur();
	SET_CONSOLE_VAR(r.DefaultFeature.MotionBlur, FMath::Min(1, (int32)MotionBlurAmount));
	if (MotionBlurAmount != 0) OnDynamicSettingsChanged.Broadcast(this);
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
	ApplyImageFidelityMode();
	ApplyColorBlindSettings();
	ApplyDynamicVibrance();
	ApplyAudioSettings();
	ApplyMotionBlur();
	ApplyLumen();
	ApplyNIS();

#if WITH_EDITOR
	if (FApp::IsGame())
#endif
	{
		if (GEngine) GEngine->DisplayGamma = GetGamma();
		SET_CONSOLE_VAR(r.SSFS, ScreenSpaceFogScattering);
		OnDynamicSettingsChanged.Broadcast(this);

		ApplyNonResolutionSettings();
	}
}

void UToroUserSettings::CacheScalabilityDefaults()
{
	ScalabilityDefaults[0] = GetOverallQuality();
	ScalabilityDefaults[1] = GetViewDistanceQuality();
	ScalabilityDefaults[2] = GetAntiAliasingQuality();
	ScalabilityDefaults[3] = GetShadowQuality();
	ScalabilityDefaults[4] = GetGlobalIlluminationQuality();
	ScalabilityDefaults[5] = GetReflectionQuality();
	ScalabilityDefaults[6] = GetPostProcessingQuality();
	ScalabilityDefaults[7] = GetTextureQuality();
	ScalabilityDefaults[8] = GetVisualEffectQuality();
	ScalabilityDefaults[9] = GetFoliageQuality();
	ScalabilityDefaults[10] = GetShadingQuality();
}

void UToroUserSettings::SetToDefaults()
{
	ShowFPS = false;
	FieldOfView = 0;
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
	NvidiaReflex = 0;
	RTXDynamicVibrance = false;
	DynamicVibranceIntensity = 0.5f;
	DynamicVibranceSaturation = 0.5f;
	NISQuality = 3;
	NISSharpness = 0.0f;
	ImageFidelityMode = EImageFidelityMode::TAA;
	FXAADithering = 3;
	TAAUpsampling = 2;
	TSRResolution = 70;
	SMAAQuality = 3;
	SMAAEdgeMode = 3;
	DLSSQuality = 4;
	DLSSFrameGeneration = 0;
	DLSSRayReconstruction = false;
	FSRQuality = 2;
	FSRSharpness = 0.0f;
	FSRFrameGeneration = false;
	XeSSQuality = 3;
	
	CheckSupportedFidelityModes();
	
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
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}
