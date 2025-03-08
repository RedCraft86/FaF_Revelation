// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SettingRowWidgets.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "UserWidgets/ToroAnimatedButtonBase.h"
#include "SettingsWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API USettingsWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	USettingsWidgetBase(const FObjectInitializer& ObjectInitializer);

	// General
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> ShowFPS;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UEditableText> Username;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> FOVOffset;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> SmoothCamera;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> SensitivityX;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> InvertX;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> SensitivityY;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> InvertY;
	// ~General

	// Video
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UResolutionRowBase> Resolution;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> Borderless;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> ResScale;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> VSync;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> MaxFrameRate;
	// ~Video

	// Visuals
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> Gamma;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> Brightness;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> FancyBloom;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> FogScattering;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> MotionBlur;
	// ~Visuals

	// Color Blind
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> ColorBlindMode;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> ColorBlindIntensity;
	// ~Color Blind

	// RTX Dynamic Vibrance
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> DynamicVibrance;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> DVIntensity;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> DVSaturation;
	// ~RTX Dynamic Vibrance

	// Nvidia Image Scaling
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> NISQuality;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> NISSharpness;
	// ~Nvidia Image Scaling

	// Anti-Aliasing / Upscaling
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImageFidelityBase> ImageFidelity;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> FXAADithering;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> TAAUpsampling;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> TSRResolution;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> SMAAQuality;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> SMAAEdgeMode;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> DLSSQuality;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> DLSSRR;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> DLSSFG;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> FSRQuality;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> FSRSharpness;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> FSRFrameGen;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> XeSSQuality;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FidelityAnim;
	// ~Anti-Aliasing / Upscaling

	// Lumen
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> LumenGI;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> LumenReflections;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> HitLightingReflect;
	// ~Lumen

	// Graphics
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> AutoDetectButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AutoDetectText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> OverallQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> ViewDistQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> AAliasingQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> ShadowQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> GIlluminationQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> ReflectionQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> PostProcessQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> TextureQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> EffectsQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> FoliageQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperRowBase> ShadingQuality;
	// ~Graphics

	// Audio
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> MasterVolume;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> MusicVolume;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> SFXVolume;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderRowBase> VoiceVolume;
	// ~Audio

	// Developer
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> DeveloperMode;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleRowBase> ViewModeUnlit;
	// ~Developer

	// Resolution Notify
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ResNotifyTimer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResRevertButton;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResConfirmButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ResNotifyAnim;
	// ~Resolution Notify

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> GeneralButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> GraphicsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> AudioButton;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> KeybindsButton;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> DeveloperButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> RefreshAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SwapScreenAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Settings)
		TObjectPtr<UUserWidget> ParentUI;

	UFUNCTION(BlueprintImplementableEvent)
		void OnRefreshUI();

	DECLARE_MULTICAST_DELEGATE(FOnRefreshDisplay);
	FOnRefreshDisplay OnRefreshDisplay;

protected:

	UPROPERTY() float AutoDetectTime;
	UPROPERTY() float ResolutionWaitTime;
	UPROPERTY(Transient) TObjectPtr<UToroUserSettings> ToroSettings;
	UPROPERTY(BlueprintReadOnly, Category = Settings) uint8 ScreenIndex;

	void SetScreenIndex(const uint8 InIndex);
	void OnGeneralButtonClicked() { SetScreenIndex(0); }
	void OnGraphicsButtonClicked() { SetScreenIndex(1); }
	void OnAudioButtonClicked() { SetScreenIndex(2); }
	void OnKeybindsButtonClicked() { SetScreenIndex(3); }
	void OnDebuggingButtonClicked() { SetScreenIndex(4); }

	void RefreshUI();
	void RefreshFidelity();
	void RefreshScalability();
	void OnResolutionChanged();
	void OnOverallQualityChanged();
	void OnAnyScalabilityChanged() const { OverallQuality->LoadValue(); }

	UFUNCTION() void OnAutoDetectClicked();
	UFUNCTION() void OnUsernameChanged(const FText& Text);
	UFUNCTION() void OnExitClicked() { DeactivateWidget(); }
	UFUNCTION() void OnConfirmResClicked()
	{
		ResolutionWaitTime = 0.0f;
		Resolution->AcceptResolution();
	}
	UFUNCTION() void OnRevertResClicked()
	{
		ResolutionWaitTime = 0.0f;
		Resolution->RevertResolution();
	}

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
