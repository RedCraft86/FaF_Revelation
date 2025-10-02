// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"
#include "UserSettings/ImageFidelity.h"
#include "ToroRuntime.h"
#include "XeFGRHI.h"

extern ENGINE_API float GAverageFPS;
extern ENGINE_API float GAverageMS;

#define OnSettingsApply(Type) OnSettingsUpdated.Broadcast(ESettingApplyType::Type);

UToroUserSettings::UToroUserSettings(): bInitialized(false)
{
	UToroUserSettings::SetToDefaults();
}

float UToroUserSettings::GetAverageMS()
{
	return GAverageMS;
}

float UToroUserSettings::GetAverageFPS()
{
	const UToroUserSettings* Settings = UToroUserSettings::Get();
	return Settings && Settings->ImageFidelity == EImageFidelityMode::XeSS ? GAverageFPS : GXeFGAverageFPS;
}

void UToroUserSettings::UpdateResolutions()
{
	SupportedResolutions.Empty();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	FullscreenRes = SupportedResolutions.Last();
	Algo::Reverse(SupportedResolutions);
}

bool UToroUserSettings::InitializeSettings(UGameInstance* GI)
{
	LoadSettings(true);
	bool bFirstLoad = false;
	if (!bInitialized)
	{
		GameInstance = GI;
		bInitialized = true;
		bFirstLoad = true;

		AutoAdjustScalability();
		SetResolutionScaleNormalized(1.0f);

		ApplyAdjustedFullscreenMode();
		SetScreenResolution(GetFullscreenResolution());
	}

	ApplySettings(false);
	return bFirstLoad;
}

void UToroUserSettings::ApplyAdjustedFullscreenMode()
{
#if WITH_EDITOR
	SetFullscreenMode(EWindowMode::Windowed);
#else
	EWindowMode::Type WindowMode = EWindowMode::WindowedFullscreen;
	if (!GetBorderless())
	{
		WindowMode = (GetScreenResolution() == GetFullscreenResolution())
			? EWindowMode::Fullscreen : EWindowMode::Windowed;
	}
	SetFullscreenMode(WindowMode);
#endif
}

void UToroUserSettings::AutoAdjustScalability()
{
	const float Res = ScalabilityQuality.ResolutionQuality;

	RunHardwareBenchmark();
	SetResolutionScaleValueEx(Res);
	ApplyHardwareBenchmarkResults();

	AutoScalability[0]	= GetOverallQuality();
	AutoScalability[1]	= GetViewDistanceQuality();
	AutoScalability[2]	= GetAntiAliasingQuality();
	AutoScalability[3]	= GetShadowQuality();
	AutoScalability[4]	= GetGlobalIlluminationQuality();
	AutoScalability[5]	= GetReflectionQuality();
	AutoScalability[6]	= GetPostProcessingQuality();
	AutoScalability[7]	= GetTextureQuality();
	AutoScalability[8]	= GetVisualEffectQuality();
	AutoScalability[9]	= GetFoliageQuality();
	AutoScalability[10] = GetShadingQuality();
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
	AudioVolume.FindOrAdd(InType) = FMath::Clamp(InVolume, 0, 150);
	ApplyAudioVolume();
}

DEFINE_PROPERTY_FUNC(bool, ShowFPS, OnSettingsApply(Dynamic))
DEFINE_PROPERTY_FUNC(bool, DeveloperMode, OnSettingsApply(Developer))
DEFINE_PROPERTY_FUNC(EGameDifficulty, Difficulty, OnSettingsApply(Difficulty))

DEFINE_PROPERTY_FUNC(bool, SmoothCamera,)
DEFINE_PROPERTY_FUNC(FVector2D, Sensitivity,)

DEFINE_PROPERTY_FUNC(EColorBlindMode, ColorBlindMode,)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, ColorBlindIntensity, 0, 10,)

DEFINE_PROPERTY_FUNC(bool, Borderless,)

DEFINE_PROPERTY_FUNC(bool, FancyBloom, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC(bool, SSFogScattering, ApplySSFogScattering();)
DEFINE_PROPERTY_FUNC_CLAMPED(float, Gamma, 0.5f, 5.0f, ApplyScreenGamma();)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, Brightness, 10, 200, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, MotionBlur, 0, 3, OnSettingsApply(Dynamic);)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, LumenGI, 0, 3, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, LumenReflections, 0, 3, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC(bool, HitLightingReflections, OnSettingsApply(Dynamic);)

DEFINE_PROPERTY_FUNC(EImageFidelityMode, ImageFidelity, ApplyImageFidelity();)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, TSRResolution, 50, 200, ApplyTSR();)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, FSRQuality, 0, 4, ApplyFSR();)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, FSRSharpness, 0, 100, ApplyFSR();)
DEFINE_PROPERTY_FUNC(bool, FSRFrameGen, ApplyFSR();)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, XeSSQuality, 0, 6, ApplyXeSS();)
DEFINE_PROPERTY_FUNC(bool, XeSSFrameGen, ApplyXeSS();)
DEFINE_PROPERTY_FUNC(bool, XeSSLowLatency, ApplyXeSS();)

void UToroUserSettings::ApplyColorBlind() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
		static_cast<EColorVisionDeficiency>(GetColorBlindMode()),
		GetColorBlindIntensity(), true, false);
}

void UToroUserSettings::ApplyScreenGamma() const
{
	if (GEngine) GEngine->DisplayGamma = GetGamma();
}

void UToroUserSettings::ApplySSFogScattering() const
{
	UToroConsoleLibrary::SetCVarBool(TEXT("r.SSFS"), GetSSFogScattering());
}

void UToroUserSettings::ApplyImageFidelity() const
{
	ImageFidelity::SetAntiAliasing(GetImageFidelity());

	ApplyTSR();
	ApplyFSR();
	ApplyXeSS();

	OnSettingsApply(UI)
}

void UToroUserSettings::ApplyAudioVolume() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UToroSettings* Settings = UToroSettings::Get();
	USoundMix* SoundMix = Settings->MainSoundMix.LoadSynchronous();
	if (!SoundMix) return;

	for (const ESoundClassType Type : TEnumRange<ESoundClassType>())
	{
		USoundClass* SoundClass = Settings->SoundClasses[static_cast<uint8>(Type)].LoadSynchronous();
		if (!SoundClass) continue;

		UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, SoundClass,
			GetAudioVolume(Type), 1.0f, 0.5f, true);
	}
}

void UToroUserSettings::ApplyTSR() const
{
	const bool bTSR = GetImageFidelity() == EImageFidelityMode::TSR;
	ImageFidelity::SetScreenPercentage(bTSR ? GetTSRResolution() : 100);
}

void UToroUserSettings::ApplyFSR() const
{
	const bool bFSR = GetImageFidelity() == EImageFidelityMode::FSR;
	ImageFidelity::FSR::SetEnabled(bFSR);
	ImageFidelity::FSR::SetQuality(GetFSRQuality());
	ImageFidelity::FSR::SetSharpness(GetFSRSharpness());
	ImageFidelity::FSR::SetFrameGenEnabled(bFSR && GetFSRFrameGen());
}

void UToroUserSettings::ApplyXeSS() const
{
	const bool bXeSS = GetImageFidelity() == EImageFidelityMode::XeSS;
	ImageFidelity::XeSS::SetMode(bXeSS, GetXeSSQuality());
	ImageFidelity::XeSS::FG::SetEnabled(bXeSS && GetXeSSFrameGen() && ImageFidelity::XeSS::FG::IsSupported());
	ImageFidelity::XeSS::LL::SetEnabled(bXeSS && GetXeSSLowLatency() && ImageFidelity::XeSS::LL::IsSupported());
}

void UToroUserSettings::SetToDefaults()
{
	ImageFidelity::FSR::Initialize();
	UpdateResolutions();

	ShowFPS = false;
	DeveloperMode = false;
	Difficulty = EGameDifficulty::Normal;
	SmoothCamera = true;
	Sensitivity = FVector2D::UnitVector;
	ColorBlindMode = EColorBlindMode::None;
	ColorBlindIntensity = 0;
	Borderless = false;
	Gamma = 2.2;
	Brightness = 100;
	FancyBloom = true;
	SSFogScattering = true;
	MotionBlur = 1;
	LumenGI = 0;
	LumenReflections = 2;
	HitLightingReflections = false;
	ImageFidelity = EImageFidelityMode::TSR;
	TSRResolution = 70;
	FSRQuality = 2;
	FSRSharpness = 0.0f;
	FSRFrameGen = false;
	XeSSQuality = 2;
	XeSSFrameGen = false;
	XeSSLowLatency = false;

	AudioVolume = {
		{ESoundClassType::Master, 100},
		{ESoundClassType::Music, 100},
		{ESoundClassType::SFX, 100},
		{ESoundClassType::Voice, 100}
	};

	Super::SetToDefaults();
}

void UToroUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
#if WITH_EDITOR
	if (FApp::IsGame())
#endif
	{
		ApplyColorBlind();
		ApplyScreenGamma();
		ApplySSFogScattering();
		ApplyImageFidelity();
		ApplyAudioVolume();
	}

	OnSettingsApply(Manual)
	Super::ApplySettings(bCheckForCommandLineOverrides);
}

UWorld* UToroUserSettings::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GameInstance ? GameInstance->GetWorld() : nullptr;
	if (!World) World = GEngine->GetCurrentPlayWorld();
	return World;
}

#undef OnSettingsApply