// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/SettingsWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Interfaces/ExitInterface.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ToroConfigs.h"

#define __SETUP_BASE(type, Property, GetterFunc, SetterFunc) \
	OnRefreshDisplay.AddUObject(Property, &USettingRowBase::LoadValue); \
	Property->AssignGetter([](UToroUserSettings* Settings)->type{ return Settings->GetterFunc(); }); \
	Property->AssignSetter([](UToroUserSettings* Settings, const type Value){ Settings->SetterFunc(Value); })

#define SETUP_TOGGLE(Property, GetterFunc, SetterFunc) __SETUP_BASE(bool, Property, GetterFunc, SetterFunc)
#define SETUP_SWAPPER(Property, GetterFunc, SetterFunc) __SETUP_BASE(int32, Property, GetterFunc, SetterFunc)
#define SETUP_SLIDER(Property, GetterFunc, SetterFunc) __SETUP_BASE(float, Property, GetterFunc, SetterFunc)

#define SETUP_QUALITY_SWAPPER(Property, GetterFunc, SetterFunc) \
	SETUP_SWAPPER(Property, GetterFunc, SetterFunc); \
	Property->OnValueChanged.AddUObject(this, &USettingsWidget::OnAnyScalabilityChanged)

#define SETUP_VOLUME_SLIDER(Property, Type) \
	OnRefreshDisplay.AddUObject(Property, &USettingRowBase::LoadValue); \
	Property->AssignGetter([](UToroUserSettings* Settings)->float{ return Settings->GetAudioVolume(ESoundClassType::Type); }); \
	Property->AssignSetter([](UToroUserSettings* Settings, const float Value){ Settings->SetAudioVolume(ESoundClassType::Type, Value); })

#define __DEV_SETUP_BASE(type, Property, GetterFunc, SetterFunc) \
	OnRefreshDisplay.AddUObject(Property, &USettingRowBase::LoadValue); \
	Property->AssignGetter([this](UToroUserSettings* Settings)->type{ return GetToroGameInstance()->GetterFunc(); }); \
	Property->AssignSetter([this](UToroUserSettings* Settings, const type Value){ GetToroGameInstance()->SetterFunc(Value); })

#define SETUP_DEV_TOGGLE(Property, GetterFunc, SetterFunc) __DEV_SETUP_BASE(bool, Property, GetterFunc, SetterFunc)
#define SETUP_DEV_SLIDER(Property, GetterFunc, SetterFunc) __DEV_SETUP_BASE(float, Property, GetterFunc, SetterFunc)
#define SETUP_DEV_SWAPPER(Property, GetterFunc, SetterFunc) __DEV_SETUP_BASE(int32, Property, GetterFunc, SetterFunc)

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ScreenIndex(0), AutoDetectTime(0.0f)
{
	ZOrder = 97;
	bAutoActivate = false;
}

void USettingsWidget::SetScreenIndex(const uint8 InIndex)
{
	if (ScreenIndex != InIndex)
	{
		ScreenIndex = InIndex;
		ScreenSwitch->SetActiveWidgetIndex(ScreenIndex);
		PlayAnimation(RefreshAnim);
	}
}

void USettingsWidget::RefreshUI()
{
	if (const UToroConfigManager* CfgManager = UToroConfigManager::Get(this))
	{
		DeveloperButton->SetVisibility(CfgManager->IsDeveloperMode()
			? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	RefreshFidelity();
	OnRefreshDisplay.Broadcast();
	PlayAnimation(RefreshAnim);
}

void USettingsWidget::RefreshFidelity() const
{
	const EImageFidelityMode Mode = ToroSettings->GetImageFidelityMode();

	TSRResolution->SetVisibility(Mode == EImageFidelityMode::TSR
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	FSRQuality->SetVisibility(Mode == EImageFidelityMode::FSR
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	FSRSharpness->SetVisibility(FSRQuality->GetVisibility());
	FSRFrameGen->SetVisibility(FSRQuality->GetVisibility());

	XeSSQuality->SetVisibility(Mode == EImageFidelityMode::XeSS
		? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USettingsWidget::RefreshScalability() const
{
	OverallQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[0];
	ViewDistQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[1];
	AAliasingQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[2];
	ShadowQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[3];
	GIlluminationQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[4];
	ReflectionQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[5];
	PostProcessQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[6];
	TextureQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[7];
	EffectsQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[8];
	FoliageQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[9];
	ShadingQuality->DefaultValue = ToroSettings->GetScalabilityDefaults()[10];
}

void USettingsWidget::OnOverallQualityChanged() const
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

void USettingsWidget::OnAutoDetect()
{
	AutoDetectTime = 5.0f;
	AutoDetectButton->SetIsEnabled(false);
	ToroSettings->AutoConfigureQuality();
}

void USettingsWidget::InitWidget()
{
	ToroSettings = UToroUserSettings::Get();

	SETUP_SWAPPER(Difficulty, GetDifficultyInt, SetDifficultyInt);

	SETUP_TOGGLE(ShowFPS, GetShowFPS, SetShowFPS);
	SETUP_TOGGLE(SmoothCamera, GetSmoothCamera, SetSmoothCamera);

	OnRefreshDisplay.AddUObject(SensitivityX, &USettingRowBase::LoadValue);
	SensitivityX->AssignGetter([](UToroUserSettings* Settings)->float { return FMath::Abs(Settings->GetSensitivityX()); });
	SensitivityX->AssignSetter([this](UToroUserSettings* Settings, const float Value)
	{
		Settings->SetSensitivityX(Value * (InvertSensiX->GetValue() ? -1.0f : 1.0f));
	});

	OnRefreshDisplay.AddUObject(InvertSensiX, &USettingRowBase::LoadValue);
	InvertSensiX->AssignGetter([](UToroUserSettings* Settings)->bool { return Settings->GetSensitivityX() < 0.0f; });
	InvertSensiX->AssignSetter([this](UToroUserSettings* Settings, const bool Value)
	{
		Settings->SetSensitivityX(SensitivityX->GetValue() * (Value ? -1.0f : 1.0f));
	});

	OnRefreshDisplay.AddUObject(SensitivityY, &USettingRowBase::LoadValue);
	SensitivityY->AssignGetter([](UToroUserSettings* Settings)->float { return FMath::Abs(Settings->GetSensitivityY()); });
	SensitivityY->AssignSetter([this](UToroUserSettings* Settings, const float Value)
	{
		Settings->SetSensitivityY(Value * (InvertSensiY->GetValue() ? -1.0f : 1.0f));
	});

	OnRefreshDisplay.AddUObject(InvertSensiY, &USettingRowBase::LoadValue);
	InvertSensiY->AssignGetter([](UToroUserSettings* Settings)->bool { return Settings->GetSensitivityY() < 0.0f; });
	InvertSensiY->AssignSetter([this](UToroUserSettings* Settings, const bool Value)
	{
		Settings->SetSensitivityY(SensitivityY->GetValue() * (Value ? -1.0f : 1.0f));
	});

	SETUP_SWAPPER(ColorBlindMode, GetColorBlindModeInt, SetColorBlindModeInt);
	SETUP_SLIDER(ColorBlindScale, GetColorBlindIntensity, SetColorBlindIntensity);

	OnRefreshDisplay.AddUObject(Borderless, &USettingRowBase::LoadValue);
	Borderless->AssignGetter([](UToroUserSettings* Settings)->bool{ return Settings->GetBorderless(); });
	Borderless->AssignSetter([](UToroUserSettings* Settings, const bool Value)
	{
		Settings->SetBorderless(Value);
		Settings->SetFullscreenMode(Settings->GetScreenResolution() == UToroUserSettings::FullscreenRes ?
#if WITH_EDITOR
		EWindowMode::Type::WindowedFullscreen
#else
		(Settings->GetBorderless() ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Fullscreen)
#endif
		: EWindowMode::Type::Windowed);
		Settings->ApplyResolutionSettings(false);
	});

	OnRefreshDisplay.AddUObject(ResolutionScale, &USettingRowBase::LoadValue);
	ResolutionScale->AssignGetter([](UToroUserSettings* Settings)->float
	{
		return Settings->GetResolutionScaleNormalized() * 100.0f;
	});
	ResolutionScale->AssignSetter([](UToroUserSettings* Settings, const float Value)
	{
		Settings->SetResolutionScaleNormalized(Value / 100.0f);
	});

	SETUP_TOGGLE(VSync, IsVSyncEnabled, SetVSyncEnabled);
	SETUP_SWAPPER(MaxFPS, GetMaxFrameRateIdx, SetMaxFrameRateIdx);

	SETUP_SLIDER(Gamma, GetGamma, SetGamma);
	SETUP_SLIDER(Brightness, GetBrightness, SetBrightness);
	SETUP_TOGGLE(FancyBloom, GetFancyBloom, SetFancyBloom);
	SETUP_TOGGLE(SSFS, GetScreenSpaceFogScattering, SetScreenSpaceFogScattering);
	SETUP_SWAPPER(MotionBlur, GetMotionBlur, SetMotionBlur);

	OnRefreshDisplay.AddUObject(ImageFidelity, &USettingRowBase::LoadValue);
	ImageFidelity->OnValueChanged.AddUObject(this, &USettingsWidget::RefreshFidelity);
	
	SETUP_SLIDER(TSRResolution, GetTSRResolution, SetTSRResolution);
	SETUP_SWAPPER(FSRQuality, GetFSRQuality, SetFSRQuality);
	SETUP_SLIDER(FSRSharpness, GetFSRSharpness, SetFSRSharpness);
	SETUP_TOGGLE(FSRFrameGen, GetFSRFrameGeneration, SetFSRFrameGeneration);
	SETUP_SWAPPER(XeSSQuality, GetXeSSQuality, SetXeSSQuality);

	AutoDetectText->SetText(FText::FromString(TEXT("Run Hardware Benchmark")));
	AutoDetectButton->OnClicked.AddDynamic(this, &USettingsWidget::OnAutoDetect);
	OverallQuality->OnValueChanged.AddUObject(this, &USettingsWidget::OnOverallQualityChanged);
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

	SETUP_DEV_TOGGLE(UnlitViewmode, IsUnlitViewmode, SetUnlitViewmode);
	SETUP_DEV_TOGGLE(InvinciblePlayer, IsPlayerInvincible, SetPlayerInvincible);

	GeneralButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnGeneralButton);
	GraphicsButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnGraphicsButton);
	AudioButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnAudioButton);
	KeybindsButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnKeybindsButton);
	DeveloperButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnDebuggingButton);
	ExitButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnExit);
}

void USettingsWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	RefreshUI();
}

void USettingsWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IExitInterface::ReturnToWidget(ParentUI, this);
}

void USettingsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
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
