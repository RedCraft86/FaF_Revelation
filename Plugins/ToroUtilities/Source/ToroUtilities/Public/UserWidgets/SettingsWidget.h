// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SettingRowWidgets.h"
#include "Framework/ToroGameInstance.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "SettingsWidget.generated.h"

class UWidgetSwitcher;

UCLASS(Abstract)
class TOROUTILITIES_API USettingsWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	USettingsWidget(const FObjectInitializer& ObjectInitializer);

	/* GENERAL */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> Difficulty;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> ShowFPS;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> SmoothCamera;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> SensitivityX;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> InvertSensiX;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> SensitivityY;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> InvertSensiY;

	/* COLOR BLIND */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> ColorBlindMode;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> ColorBlindScale;

	/* VIDEO */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UResolutionSettingRow> Resolution;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> Borderless;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> ResolutionScale;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> VSync;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> MaxFPS;

	/* VISUALS */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> Gamma;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> Brightness;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> FancyBloom;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> SSFS;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> MotionBlur;

	/* LUMEN */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> LumenGI;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> LumenReflect;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> HitLighting;

	/* IMAGE FIDELITY */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImageFidelitySettingRow> ImageFidelity;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> TSRResolution;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> FSRQuality;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> FSRSharpness;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> FSRFrameGen;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> XeSSQuality;

	/* GRAPHICS */
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> AutoDetectButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AutoDetectText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> OverallQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> ViewDistQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> AAliasingQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> ShadowQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> GIlluminationQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> ReflectionQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> PostProcessQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> TextureQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> EffectsQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> FoliageQuality;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USwapperSettingRow> ShadingQuality;

	/* AUDIO */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> MasterVolume;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> MusicVolume;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> SFXVolume;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USliderSettingRow> VoiceVolume;

	/* DEVELOPER */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> UnlitViewmode;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToggleSettingRow> InvinciblePlayer;

	/* MENU */

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> GeneralButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> GraphicsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> AudioButton;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> KeybindsButton;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> DeveloperButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> ScreenSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> RefreshAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Settings)
		TObjectPtr<UUserWidget> ParentUI;

	DECLARE_MULTICAST_DELEGATE(FOnRefreshDisplay);
	FOnRefreshDisplay OnRefreshDisplay;

protected:

	UPROPERTY() uint8 ScreenIndex;
	UPROPERTY() float AutoDetectTime;
	UPROPERTY(Transient) TObjectPtr<UToroUserSettings> ToroSettings;

	void SetScreenIndex(const uint8 InIndex);
	UFUNCTION() void OnGeneralButton() { SetScreenIndex(0); }
	UFUNCTION() void OnGraphicsButton() { SetScreenIndex(1); }
	UFUNCTION() void OnAudioButton() { SetScreenIndex(2); }
	UFUNCTION() void OnKeybindsButton() { SetScreenIndex(3); }
	UFUNCTION() void OnDebuggingButton() { SetScreenIndex(4); }

	void RefreshUI();
	void RefreshFidelity() const;
	void RefreshScalability() const;
	void OnOverallQualityChanged() const;
	void OnAnyScalabilityChanged() const { OverallQuality->LoadValue(); }
	UToroGameInstance* GetToroGameInstance() const { return GetWorld()->GetGameInstance<UToroGameInstance>(); }

	UFUNCTION() void OnAutoDetect();
	UFUNCTION() void OnExit() { DeactivateWidget(); }

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
