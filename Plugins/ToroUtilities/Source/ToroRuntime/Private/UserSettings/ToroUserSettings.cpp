// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "ToroRuntimeSettings.h"
#include "AudioDevice.h"

UToroUserSettings::UToroUserSettings() : ShowFPS(false), FieldOfView(0), SmoothCamera(true)
	, SensitivityX(1.0f), SensitivityY(1.0f), Gamma(2.2f), Brightness(100), FancyBloom(true)
	, ScreenSpaceFogScattering(true), MotionBlur(2), LumenGI(0), LumenReflection(2)
	, HitLightingReflections(false), ColorBlindMode(EColorBlindMode::None), ColorBlindIntensity(0)
	, NvidiaReflex(0), RTXDynamicVibrance(false), DynamicVibranceIntensity(0.5f)
	, DynamicVibranceSaturation(0.5f), ImageFidelityMode(EImageFidelityMode::TAA), FXAADithering(3)
	, TAAUpsampling(2), TSRResolution(70), SMAAQuality(3), SMAAEdgeMode(3), DLSSQuality(3)
	, DLSSSharpness(0.0f), DLSSRayReconstruction(false), DLSSFrameGeneration(false), FSRQuality(2)
	, FSRSharpness(0.0f), FSRFrameGeneration(false), XeSSQuality(3), NISQuality(3), NISSharpness(0.0f)
	, NISScreenPercentage(100.0f)
{
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

DEFINE_SETTER(bool, ShowFPS, ApplyShowFPS();)
DEFINE_SETTER_BASIC(FString, Username)

DEFINE_SETTER_DYNAMIC(uint8, FieldOfView)
DEFINE_SETTER_BASIC(bool, SmoothCamera)
DEFINE_SETTER_BASIC(float, SensitivityX)
DEFINE_SETTER_BASIC(float, SensitivityY)

DEFINE_SETTER(float, Gamma, if (GEngine) GEngine->DisplayGamma = GetGamma();)
DEFINE_SETTER(uint8, Brightness, ApplyBrightness();)
DEFINE_SETTER_DYNAMIC(bool, FancyBloom)
DEFINE_SETTER_CONSOLE(bool, ScreenSpaceFogScattering, r.SSFS)
DEFINE_SETTER(uint8, MotionBlur, ApplyMotionBlur();)

DEFINE_SETTER(uint8, LumenGI, ApplyLumen();)
DEFINE_SETTER(uint8, LumenReflection, ApplyLumen();)
DEFINE_SETTER(bool, HitLightingReflections, ApplyLumen();)

DEFINE_SETTER(EColorBlindMode, ColorBlindMode, ApplyColorBlindSettings();)
DEFINE_SETTER(uint8, ColorBlindIntensity, ApplyColorBlindSettings();)

DEFINE_SETTER(uint8, NvidiaReflex, 
	
)

DEFINE_SETTER(bool, RTXDynamicVibrance, ApplyDynamicVibrance();)
DEFINE_SETTER(float, DynamicVibranceIntensity, ApplyDynamicVibrance();)
DEFINE_SETTER(float, DynamicVibranceSaturation, ApplyDynamicVibrance();)

DEFINE_SETTER(EImageFidelityMode, ImageFidelityMode, ApplyImageFidelityMode();)

DEFINE_SETTER(uint8, FXAADithering, SET_CONSOLE_VAR(r.FXAA.Quality, GetFXAADithering() + 1))
DEFINE_SETTER_CONSOLE(uint8, TAAUpsampling, r.TemporalAA.Quality)
DEFINE_SETTER_CONSOLE(uint8, TSRResolution, r.ScreenPercentage)

DEFINE_SETTER_CONSOLE(uint8, SMAAQuality, r.SMAA.Quality)
DEFINE_SETTER_CONSOLE(uint8, SMAAEdgeMode, r.SMAA.EdgeDetector)

DEFINE_SETTER(uint8, DLSSQuality, ApplyDLSS();)
DEFINE_SETTER(float, DLSSSharpness, ApplyDLSS();)
DEFINE_SETTER(bool, DLSSRayReconstruction, ApplyDLSS();)
DEFINE_SETTER(bool, DLSSFrameGeneration, 
	
)

DEFINE_SETTER(uint8, FSRQuality, ApplyFSR();)
DEFINE_SETTER(float, FSRSharpness, ApplyFSR();)
DEFINE_SETTER_CONSOLE(bool, FSRFrameGeneration, r.FidelityFX.FI.Enabled)

DEFINE_SETTER_CONSOLE(uint8, XeSSQuality, r.XeSS.Quality)

DEFINE_SETTER(uint8, NISQuality, ApplyNIS();)
DEFINE_SETTER(float, NISSharpness, ApplyNIS();)
DEFINE_SETTER(float, NISScreenPercentage, ApplyNIS();)

void UToroUserSettings::ApplyNIS() const
{
}

void UToroUserSettings::ApplyFSR() const
{
	SET_CONSOLE_VAR(r.FidelityFX.FSR3.QualityMode, GetFSRQuality())
	SET_CONSOLE_VAR(r.FidelityFX.FSR3.Sharpness, GetFSRSharpness())
}

void UToroUserSettings::ApplyDLSS() const
{
}

void UToroUserSettings::ApplyImageFidelityMode()
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	if (SupportedFidelityModes.Contains(ImageFidelityMode))
	{
		uint8 AAMode = 0;
		bool bSMAA = false, bDLSS = false, bFSR = false, bXeSS = false, bNIS = false;
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
			bDLSS = true;
			ApplyDLSS();
			break;
		case EImageFidelityMode::FSR:
			bFSR = true;
			ApplyFSR();
			break;
		case EImageFidelityMode::XeSS:
			bXeSS = true;
			SET_CONSOLE_VAR(r.XeSS.Quality, GetXeSSQuality())
			break;
		case EImageFidelityMode::NIS:
			bNIS = true;
			ApplyNIS();
			break;
		}
		
		SET_CONSOLE_VAR(r.XeSS.Enabled, bXeSS)
		SET_CONSOLE_VAR(r.SMAA, bSMAA)
		
		SET_CONSOLE_VAR(r.AntiAliasingMethod, AAMode)
		SET_CONSOLE_VAR(r.ScreenPercentage, AAMode == 4 ? TSRResolution : 100.0f);
		
		SET_CONSOLE_VAR(r.FidelityFX.FSR3.Enable, bFSR)
		SET_CONSOLE_VAR(r.FidelityFX.FI.Enabled, bFSR ? FSRFrameGeneration : false)

		// TODO: Add support for DLSS and NIS
		
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
}

void UToroUserSettings::ApplyAudioSettings() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (USoundMix* SoundMix = Settings ? Settings->MainSoundMix.LoadSynchronous() : nullptr)
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

void UToroUserSettings::ApplyBrightness() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (UMaterialParameterCollection* MPC = Settings ? Settings->MainMPC.LoadSynchronous() : nullptr)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC,
			TEXT("Brightness"), GetBrightness());
	}
}

void UToroUserSettings::ApplyMotionBlur() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const uint8 MotionBlur = GetMotionBlur();
	SET_CONSOLE_VAR(r.DefaultFeature.MotionBlur, FMath::Min(1, (int32)MotionBlur));
	if (MotionBlur != 0) OnDynamicSettingsChanged.Broadcast(this);
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

void UToroUserSettings::ApplyShowFPS() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
}

void UToroUserSettings::CheckSupportedFidelityModes()
{
	SupportedFidelityModes = {EImageFidelityMode::FXAA, EImageFidelityMode::TAA,
		EImageFidelityMode::TSR, EImageFidelityMode::SMAA, EImageFidelityMode::FSR};
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
	CheckSupportedFidelityModes();
	ApplyImageFidelityMode();
	ApplyColorBlindSettings();
	ApplyDynamicVibrance();
	ApplyAudioSettings();
	ApplyBrightness();
	ApplyMotionBlur();
	ApplyLumen();
	ApplyShowFPS();

#if WITH_EDITOR
	if (FApp::IsGame())
#endif
	{
		if (GEngine) GEngine->DisplayGamma = GetGamma();
		SET_CONSOLE_VAR(r.SSFS, ScreenSpaceFogScattering);
	}
	
	Super::SetToDefaults();
}

void UToroUserSettings::ApplyNonResolutionSettings()
{
	OnSettingsApplied.Broadcast(this);
	Super::ApplyNonResolutionSettings();
	SaveSettings();
}

UWorld* UToroUserSettings::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}
