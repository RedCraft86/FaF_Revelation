// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst
#include "UserSettings/SettingsWidgetBase.h"
#include "UserSettings/ToroUserSettings.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ToroConfigManager.h"
#include "GeneralInterface.h"

#define SteamAudioCfgSection TEXT("[/Script/SteamAudio.SteamAudioSettings]")

#define __SETUP_BASE(type, Property, GetterFunc, SetterFunc) \
	OnRefreshDisplay.AddUObject(Property, &USettingRowBase::LoadValue); \
	Property->AssignGetter([this]()->type{ return ToroSettings->GetterFunc(); }); \
	Property->AssignSetter([this](const type Value){ ToroSettings->SetterFunc(Value); })

#define SETUP_TOGGLE(Property, GetterFunc, SetterFunc) __SETUP_BASE(bool, Property, GetterFunc, SetterFunc)
#define SETUP_SWAPPER(Property, GetterFunc, SetterFunc) __SETUP_BASE(int32, Property, GetterFunc, SetterFunc)
#define SETUP_SLIDER(Property, GetterFunc, SetterFunc) __SETUP_BASE(float, Property, GetterFunc, SetterFunc)

#define SETUP_QUALITY_SWAPPER(Property, GetterFunc, SetterFunc) \
	SETUP_SWAPPER(Property, GetterFunc, SetterFunc); \
	Property->OnValueChanged.AddUObject(this, &USettingsWidgetBase::OnAnyScalabilityChanged)

#define SETUP_VOLUME_SLIDER(Property, Type) \
	OnRefreshDisplay.AddUObject(Property, &USettingRowBase::LoadValue); \
	Property->AssignGetter([this]()->float{ return ToroSettings->GetAudioVolume(ESoundClassType::Type); }); \
	Property->AssignSetter([this](const float Value){ ToroSettings->SetAudioVolume(ESoundClassType::Type, Value); })

#define __DEV_SETUP_BASE(type, Property, GetterFunc, SetterFunc) \
	OnRefreshDisplay.AddUObject(Property, &USettingRowBase::LoadValue); \
	Property->AssignGetter([this]()->type{ return GetWorld()->GetGameInstance<UToroGameInstance>()->GetterFunc(); }); \
	Property->AssignSetter([this](const type Value){ GetWorld()->GetGameInstance<UToroGameInstance>()->SetterFunc(Value); })

#define SETUP_DEV_TOGGLE(Property, GetterFunc, SetterFunc) __DEV_SETUP_BASE(bool, Property, GetterFunc, SetterFunc)
#define SETUP_DEV_SLIDER(Property, GetterFunc, SetterFunc) __DEV_SETUP_BASE(float, Property, GetterFunc, SetterFunc)
#define SETUP_DEV_SWAPPER(Property, GetterFunc, SetterFunc) __DEV_SETUP_BASE(int32, Property, GetterFunc, SetterFunc)

USettingsWidgetBase::USettingsWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), AutoDetectTime(0.0f), ResolutionWaitTime(0.0f), ScreenIndex(0)
{
	ZOrder = 58;
}

void USettingsWidgetBase::SetScreenIndex(const uint8 InIndex)
{
	if (ScreenIndex != InIndex && !IsAnimationPlaying(SwapScreenAnim))
	{
		ScreenIndex = InIndex;
		PlayAnimation(SwapScreenAnim);
	}
}

void USettingsWidgetBase::RefreshUI()
{
	if (const UToroConfigManager* CfgManager = UToroConfigManager::Get(this))
	{
		DeveloperButton->SetVisibility(CfgManager->IsDeveloperMode()
			? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	PlayAnimation(RefreshAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.5f);
	RefreshFidelity();

	const bool bDV = ToroSettings->IsDynamicVibranceSupported();
	DynamicVibrance->SetVisibility(bDV ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	DVIntensity->SetVisibility(DynamicVibrance->GetVisibility());
	DVSaturation->SetVisibility(DynamicVibrance->GetVisibility());

	const bool bNIS = ToroSettings->IsNISSupported();
	NISQuality->SetVisibility(bNIS ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	NISSharpness->SetVisibility(NISQuality->GetVisibility());

	OnRefreshUI();
	OnRefreshDisplay.Broadcast();
}

void USettingsWidgetBase::RefreshFidelity()
{
	PlayAnimation(FidelityAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.5f);
	const EImageFidelityMode Mode = ToroSettings->GetImageFidelityMode();

	FXAADithering->SetVisibility(Mode == EImageFidelityMode::FXAA
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	TAAUpsampling->SetVisibility(Mode == EImageFidelityMode::TAA
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	TSRResolution->SetVisibility(Mode == EImageFidelityMode::TSR
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	SMAAQuality->SetVisibility(Mode == EImageFidelityMode::SMAA
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	SMAAEdgeMode->SetVisibility(SMAAQuality->GetVisibility());

	DLSSQuality->SetVisibility(Mode == EImageFidelityMode::DLSS
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	DLSSRR->SetVisibility(DLSSQuality->GetVisibility());
	DLSSFG->SetVisibility(DLSSQuality->GetVisibility());

	FSRQuality->SetVisibility(Mode == EImageFidelityMode::FSR
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	FSRSharpness->SetVisibility(FSRQuality->GetVisibility());
	FSRFrameGen->SetVisibility(FSRQuality->GetVisibility());

	XeSSQuality->SetVisibility(Mode == EImageFidelityMode::XeSS
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USettingsWidgetBase::RefreshScalability()
{
	OverallQuality->DefaultValue = ToroSettings->ScalabilityDefaults[0];
	ViewDistQuality->DefaultValue = ToroSettings->ScalabilityDefaults[1];
	AAliasingQuality->DefaultValue = ToroSettings->ScalabilityDefaults[2];
	ShadowQuality->DefaultValue = ToroSettings->ScalabilityDefaults[3];
	GIlluminationQuality->DefaultValue = ToroSettings->ScalabilityDefaults[4];
	ReflectionQuality->DefaultValue = ToroSettings->ScalabilityDefaults[5];
	PostProcessQuality->DefaultValue = ToroSettings->ScalabilityDefaults[6];
	TextureQuality->DefaultValue = ToroSettings->ScalabilityDefaults[7];
	EffectsQuality->DefaultValue = ToroSettings->ScalabilityDefaults[8];
	FoliageQuality->DefaultValue = ToroSettings->ScalabilityDefaults[9];
	ShadingQuality->DefaultValue = ToroSettings->ScalabilityDefaults[10];
}

void USettingsWidgetBase::OnResolutionChanged()
{
	PlayAnimation(ResNotifyAnim);
	ResolutionWaitTime = 5.0f;
}

void USettingsWidgetBase::OnOverallQualityChanged()
{
	ViewDistQuality->LoadValue();
	AAliasingQuality->LoadValue();
	ShadowQuality->LoadValue();
	GIlluminationQuality->LoadValue();
	ReflectionQuality->LoadValue();
	PostProcessQuality->LoadValue();
	TextureQuality->LoadValue();
	EffectsQuality->LoadValue();
	FoliageQuality->LoadValue();
	ShadingQuality->LoadValue();
}

void USettingsWidgetBase::OnAutoDetectClicked()
{
	AutoDetectTime = 5.0f;
	AutoDetectButton->SetIsEnabled(false);
	ToroSettings->AutoConfigureQuality();
}

void USettingsWidgetBase::OnUsernameChanged(const FText& Text)
{
	if (Text.IsEmptyOrWhitespace()) Username->SetText(INVTEXT("Player"));
	if (ToroSettings) ToroSettings->SetUsername(Text.ToString());
}

void USettingsWidgetBase::InitWidget()
{
	Super::InitWidget();
	ToroSettings = UToroUserSettings::Get();

	SETUP_TOGGLE(ShowFPS, GetShowFPS, SetShowFPS);

	Username->SetText(FText::FromString(ToroSettings->GetUsername()));
	Username->OnTextChanged.AddUniqueDynamic(this, &USettingsWidgetBase::OnUsernameChanged);

	SETUP_SLIDER(FOVOffset, GetFieldOfViewOffset, SetFieldOfViewOffset);
	SETUP_TOGGLE(SmoothCamera, GetSmoothCamera, SetSmoothCamera);

	OnRefreshDisplay.AddUObject(SensitivityX, &USettingRowBase::LoadValue);
	SensitivityX->AssignGetter([this]()->float { return FMath::Abs(ToroSettings->GetSensitivityX()); });
	SensitivityX->AssignSetter([this](const float Value)
	{
		ToroSettings->SetSensitivityX(Value * (InvertX->GetValue() ? -1.0f : 1.0f));
	});

	OnRefreshDisplay.AddUObject(InvertX, &USettingRowBase::LoadValue);
	InvertX->AssignGetter([this]()->bool { return ToroSettings->GetSensitivityX() < 0.0f; });
	InvertX->AssignSetter([this](const bool Value)
	{
		ToroSettings->SetSensitivityX(SensitivityX->GetValue() * (Value ? -1.0f : 1.0f));
	});

	OnRefreshDisplay.AddUObject(SensitivityY, &USettingRowBase::LoadValue);
	SensitivityY->AssignGetter([this]()->float { return FMath::Abs(ToroSettings->GetSensitivityY()); });
	SensitivityY->AssignSetter([this](const float Value)
	{
		ToroSettings->SetSensitivityY(Value * (InvertY->GetValue() ? -1.0f : 1.0f));
	});

	OnRefreshDisplay.AddUObject(InvertY, &USettingRowBase::LoadValue);
	InvertY->AssignGetter([this]()->bool { return ToroSettings->GetSensitivityY() < 0.0f; });
	InvertY->AssignSetter([this](const bool Value)
	{
		ToroSettings->SetSensitivityY(SensitivityY->GetValue() * (Value ? -1.0f : 1.0f));
	});

	Resolution->OnValueChanged.AddUObject(this, &USettingsWidgetBase::OnResolutionChanged);
	OnRefreshDisplay.AddUObject(Borderless, &USettingRowBase::LoadValue);
	Borderless->AssignGetter([this]()->bool{ return ToroSettings->GetBorderless(); });
	Borderless->AssignSetter([this](const bool Value)
	{
		ToroSettings->SetBorderless(Value);
		ToroSettings->SetFullscreenMode(ToroSettings->GetScreenResolution() == UToroUserSettings::FullscreenRes ?
#if WITH_EDITOR
		EWindowMode::Type::WindowedFullscreen
#else
		(ToroSettings->GetBorderless() ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Fullscreen)
#endif
		: EWindowMode::Type::Windowed);
		ToroSettings->ApplyResolutionSettings(false);
	});

	OnRefreshDisplay.AddUObject(ResScale, &USettingRowBase::LoadValue);
	ResScale->AssignGetter([this]()->float{ return ToroSettings->GetResolutionScaleNormalized() * 100.0f; });
	ResScale->AssignSetter([this](const float Value){ ToroSettings->SetResolutionScaleNormalized(Value / 100.0f); });

	SETUP_TOGGLE(VSync, IsVSyncEnabled, SetVSyncEnabled);
	SETUP_SWAPPER(MaxFrameRate, GetMaxFrameRateIdx, SetMaxFrameRateIdx);

	SETUP_SLIDER(Gamma, GetGamma, SetGamma);
	SETUP_SLIDER(Brightness, GetBrightness, SetBrightness);
	SETUP_TOGGLE(FancyBloom, GetFancyBloom, SetFancyBloom);
	SETUP_TOGGLE(FogScattering, GetScreenSpaceFogScattering, SetScreenSpaceFogScattering);
	SETUP_SWAPPER(MotionBlur, GetMotionBlur, SetMotionBlur);

	SETUP_SWAPPER(ColorBlindMode, GetColorBlindModeInt, SetColorBlindModeInt);
	SETUP_SLIDER(ColorBlindIntensity, GetColorBlindIntensity, SetColorBlindIntensity);

	SETUP_TOGGLE(DynamicVibrance, GetRTXDynamicVibrance, SetRTXDynamicVibrance);
	SETUP_SLIDER(DVIntensity, GetDynamicVibranceIntensity, SetDynamicVibranceIntensity);
	SETUP_SLIDER(DVSaturation, GetDynamicVibranceSaturation, SetDynamicVibranceSaturation);

	SETUP_SWAPPER(NISQuality, GetNISQuality, SetNISQuality);
	SETUP_SLIDER(NISSharpness, GetNISSharpness, SetNISSharpness);

	OnRefreshDisplay.AddUObject(ImageFidelity, &USettingRowBase::LoadValue);
	ImageFidelity->OnValueChanged.AddUObject(this, &USettingsWidgetBase::RefreshFidelity);

	SETUP_SWAPPER(FXAADithering, GetFXAADithering, SetFXAADithering);
	SETUP_SWAPPER(TAAUpsampling, GetTAAUpsampling, SetTAAUpsampling);
	SETUP_SLIDER(TSRResolution, GetTSRResolution, SetTSRResolution);
	SETUP_SWAPPER(SMAAQuality, GetSMAAQuality, SetSMAAQuality);
	SETUP_SWAPPER(SMAAEdgeMode, GetSMAAEdgeMode, SetSMAAEdgeMode);
	SETUP_SWAPPER(DLSSQuality, GetDLSSQuality, SetDLSSQuality);
	SETUP_TOGGLE(DLSSRR, GetDLSSRayReconstruction, SetDLSSRayReconstruction);
	SETUP_SWAPPER(DLSSFG, GetDLSSFrameGeneration, SetDLSSFrameGeneration);
	SETUP_SWAPPER(FSRQuality, GetFSRQuality, SetFSRQuality);
	SETUP_SLIDER(FSRSharpness, GetFSRSharpness, SetFSRSharpness);
	SETUP_TOGGLE(FSRFrameGen, GetFSRFrameGeneration, SetFSRFrameGeneration);
	SETUP_SWAPPER(XeSSQuality, GetXeSSQuality, SetXeSSQuality);

	AutoDetectText->SetText(FText::FromString(TEXT("Run Hardware Benchmark")));
	AutoDetectButton->OnClicked.AddDynamic(this, &USettingsWidgetBase::OnAutoDetectClicked);
	OverallQuality->OnValueChanged.AddUObject(this, &USettingsWidgetBase::OnOverallQualityChanged);
	SETUP_SWAPPER(OverallQuality, GetOverallQuality, SetOverallQuality);
	SETUP_QUALITY_SWAPPER(ViewDistQuality, GetViewDistanceQuality, SetViewDistanceQuality);
	SETUP_QUALITY_SWAPPER(AAliasingQuality, GetAntiAliasingQuality, SetAntiAliasingQuality);
	SETUP_QUALITY_SWAPPER(ShadowQuality, GetShadowQuality, SetShadowQuality);
	SETUP_QUALITY_SWAPPER(GIlluminationQuality, GetGlobalIlluminationQuality, SetGlobalIlluminationQuality);
	SETUP_QUALITY_SWAPPER(ReflectionQuality, GetReflectionQuality, SetReflectionQuality);
	SETUP_QUALITY_SWAPPER(PostProcessQuality, GetPostProcessingQuality, SetPostProcessingQuality);
	SETUP_QUALITY_SWAPPER(TextureQuality, GetTextureQuality, SetTextureQuality);
	SETUP_QUALITY_SWAPPER(EffectsQuality, GetVisualEffectQuality, SetVisualEffectQuality);
	SETUP_QUALITY_SWAPPER(FoliageQuality, GetFoliageQuality, SetFoliageQuality);
	SETUP_QUALITY_SWAPPER(ShadingQuality, GetShadingQuality, SetShadingQuality);

	SETUP_VOLUME_SLIDER(MasterVolume, Master);
	SETUP_VOLUME_SLIDER(MusicVolume, Music);
	SETUP_VOLUME_SLIDER(SFXVolume, SoundFX);
	SETUP_VOLUME_SLIDER(VoiceVolume, Voice);

	SETUP_DEV_TOGGLE(DeveloperMode, IsDeveloperMode, SetDeveloperMode);
	SETUP_DEV_TOGGLE(ViewModeUnlit, IsUnlitViewmode, SetUnlitViewmode);

	ResRevertButton->OnClicked.AddUniqueDynamic(this, &USettingsWidgetBase::OnRevertResClicked);
	ResConfirmButton->OnClicked.AddUniqueDynamic(this, &USettingsWidgetBase::OnConfirmResClicked);
	GeneralButton->OnClicked.AddUObject(this, &USettingsWidgetBase::OnGeneralButtonClicked);
	GraphicsButton->OnClicked.AddUObject(this, &USettingsWidgetBase::OnGraphicsButtonClicked);
	AudioButton->OnClicked.AddUObject(this, &USettingsWidgetBase::OnAudioButtonClicked);
	KeybindsButton->OnClicked.AddUObject(this, &USettingsWidgetBase::OnKeybindsButtonClicked);
	DeveloperButton->OnClicked.AddUObject(this, &USettingsWidgetBase::OnDebuggingButtonClicked);
	ExitButton->OnClicked.AddUniqueDynamic(this, &USettingsWidgetBase::OnExitClicked);
}

void USettingsWidgetBase::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	RefreshUI();
}

void USettingsWidgetBase::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IToroGeneralInterface::Return(ParentUI);
}

void USettingsWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (ResolutionWaitTime > 0.0f)
	{
		ResolutionWaitTime -= InDeltaTime;
		if (ResolutionWaitTime <= 0.0f) OnRevertResClicked();
		else ResNotifyTimer->SetText(FText::Format(INVTEXT("{0}"), FMath::CeilToInt(ResolutionWaitTime)));
	}

	if (AutoDetectTime > 0.0f)
	{
		AutoDetectTime -= InDeltaTime;
		if (AutoDetectTime <= 0.0f)
		{
			AutoDetectButton->SetIsEnabled(true);
			AutoDetectText->SetText(FText::FromString(TEXT("Run Hardware Benchmark")));
		}
		else
		{
			AutoDetectText->SetText(FText::FromString(FString::Printf(TEXT("Run Hardware Benchmark (%ds)"),
				FMath::CeilToInt(AutoDetectTime))));
		}
	}
}
