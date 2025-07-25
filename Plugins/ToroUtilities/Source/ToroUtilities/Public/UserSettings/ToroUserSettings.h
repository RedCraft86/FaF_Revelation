﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.h"
#include "UpscalerWrappers.h"
#include "Libraries/ToroConsoleLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "ToroUserSettings.generated.h"

#define SET_CONSOLE_VAR(Command, Value) \
	if (IConsoleVariable* CVar = UToroConsoleLibrary::FindCVar(TEXT(#Command))) \
	{ \
		CVar->Set(Value, ECVF_SetByConsole); \
	} 

#define DECLARE_PROPERTY_FUNC(Type, Name) \
	void Set##Name(const Type InValue); \
	Type Get##Name() const { return Name; } \

#define DECLARE_PROPERTY_FUNC_CLAMPED(Type, Name, Min, Max) \
	void Set##Name(const Type InValue); \
	Type Get##Name() const { return FMath::Clamp(Name, Min, Max); } \

#define DECLARE_CONVERTABLE_FUNC(Type, Name, AltType, AltTypeName) \
	DECLARE_PROPERTY_FUNC(Type, Name) \
	void Set##Name##AltTypeName(const AltType InValue) { Set##Name(static_cast<Type>(InValue)); } \
	AltType Get##Name##AltTypeName() const { return static_cast<AltType>(Name); }

#define DEFINE_SETTER(Type, Name, Code) \
	void UToroUserSettings::Set##Name(const Type InValue) \
	{ \
		if (Name != InValue) \
		{ \
			Name = InValue; \
			Code \
		} \
	}
#define DEFINE_SETTER_BASIC(Type, Name) DEFINE_SETTER(Type, Name, )
#define DEFINE_SETTER_DYNAMIC(Type, Name) DEFINE_SETTER(Type, Name, OnDynamicSettingsChanged.Broadcast(this);)
#define DEFINE_SETTER_CONSOLE(Type, Name, Command) \
	DEFINE_SETTER(Type, Name, \
	SET_CONSOLE_VAR(Command, Get##Name()))

class UToroGameInstance;

UCLASS(NotBlueprintable, BlueprintType)
class TOROUTILITIES_API UToroUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UToroUserSettings();

	static UToroUserSettings* Get() { return Cast<UToroUserSettings>(GEngine->GameUserSettings); }

	static inline TArray<FIntPoint> SupportedResolutions = {};
	static inline FIntPoint FullscreenRes = {1920, 1080};
	static void CheckSupportedResolutions();

	static inline TArray<EImageFidelityMode> SupportedFidelityModes = {};
	static void CheckSupportedFidelityModes();

	DECLARE_MULTICAST_DELEGATE_OneParam(FUserSettingsDelegate, const UToroUserSettings*)
	FUserSettingsDelegate OnDynamicSettingsChanged;
	FUserSettingsDelegate OnSettingsApplied;
	FUserSettingsDelegate RefreshUI;

	void AutoConfigureQuality();
	void SetOverallQuality(const uint8 InValue);
	uint8 GetOverallQuality() const;

	void SetMaxFrameRateIdx(const uint8 InIndex);
	uint8 GetMaxFrameRateIdx() const;

	void SetAudioVolume(const ESoundClassType InType, const uint8 InVolume);
	uint8 GetAudioVolume(const ESoundClassType InType) const
	{
		return FMath::Clamp(AudioVolume.FindRef(InType), 25, 150);
	}

	TArray<uint8>& GetScalabilityDefaults() { return ScalabilityDefaults; }
	
	DECLARE_PROPERTY_FUNC(EGameDifficulty, Difficulty)

	DECLARE_PROPERTY_FUNC(bool, ShowFPS)
	DECLARE_PROPERTY_FUNC(bool, SmoothCamera)
	DECLARE_PROPERTY_FUNC_CLAMPED(float, SensitivityX, -2.0f, 2.0f)
	DECLARE_PROPERTY_FUNC_CLAMPED(float, SensitivityY, -2.0f, 2.0f)
	
	DECLARE_CONVERTABLE_FUNC(EColorBlindMode, ColorBlindMode, uint8, Int)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, ColorBlindIntensity, 0, 10)

	DECLARE_PROPERTY_FUNC(bool, Borderless)
	
	DECLARE_PROPERTY_FUNC_CLAMPED(float, Gamma, 0.5f, 5.0f)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, Brightness, 10, 100)
	DECLARE_PROPERTY_FUNC(bool, FancyBloom)
	DECLARE_PROPERTY_FUNC(bool, ScreenSpaceFogScattering)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, MotionBlur, 0, 3)

	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, LumenGI, 0, 3)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, LumenReflection, 0, 3)
	DECLARE_PROPERTY_FUNC(bool, HitLightingReflections)
	
	DECLARE_CONVERTABLE_FUNC(EImageFidelityMode, ImageFidelityMode, uint8, Int)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, TSRResolution, 25, 200)

	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, FSRQuality, 0, 4)
	DECLARE_PROPERTY_FUNC_CLAMPED(float, FSRSharpness, 0.0f, 1.0f)
	DECLARE_PROPERTY_FUNC(bool, FSRFrameGeneration)
	
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, XeSSQuality, 0, 6)

	void InitSettings(UGameInstance* InGameInstance);

protected:

	void ApplyFSR() const;
	void ApplyImageFidelityMode();

	void ApplyColorBlindSettings() const;
	void ApplyAudioSettings() const;
	void ApplyMotionBlur() const;
	void ApplyLumen() const;

	void ReapplySettings();
	void CacheScalabilityDefaults();

	virtual void SetToDefaults() override;
	virtual void ApplyNonResolutionSettings() override;
	virtual void LoadSettings(bool bForceReload) override;
	virtual UWorld* GetWorld() const override;

	UPROPERTY(Transient) TObjectPtr<UGameInstance> GameInstance;

	/* Misc */
	UPROPERTY(Config) EGameDifficulty Difficulty;
	UPROPERTY(Config) bool ShowFPS;
	UPROPERTY(Config) bool Borderless;
	UPROPERTY(Config) TMap<ESoundClassType, uint8> AudioVolume;

	/* Camera */
	UPROPERTY(Config) bool SmoothCamera;
	UPROPERTY(Config) float SensitivityX;
	UPROPERTY(Config) float SensitivityY;

	/* Graphics */
	UPROPERTY(Config) float Gamma;
	UPROPERTY(Config) uint8 Brightness;
	UPROPERTY(Config) bool FancyBloom;
	UPROPERTY(Config) bool ScreenSpaceFogScattering;
	UPROPERTY(Config) uint8 MotionBlur;

	// Global Illumination
	UPROPERTY(Config) uint8 LumenGI;

	// Reflections
	UPROPERTY(Config) uint8 LumenReflection;
	UPROPERTY(Config) bool HitLightingReflections;

	/* Color Correction */
	UPROPERTY(Config) EColorBlindMode ColorBlindMode;
	UPROPERTY(Config) uint8 ColorBlindIntensity;

	/* Anti-Aliasing / Upscaling */
	UPROPERTY(Config) EImageFidelityMode ImageFidelityMode;

	// Native AA / Upscaling
	UPROPERTY(Config) uint8 TSRResolution;

	// FSR
	UPROPERTY(Config) uint8 FSRQuality;
	UPROPERTY(Config) float FSRSharpness;
	UPROPERTY(Config) bool FSRFrameGeneration; // Frame Interpolation

	// XeSS
	UPROPERTY(Config) uint8 XeSSQuality;

	// Default values for scalability. Will get overridden by automatic configuration
	UPROPERTY(Config) TArray<uint8> ScalabilityDefaults{3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
};
