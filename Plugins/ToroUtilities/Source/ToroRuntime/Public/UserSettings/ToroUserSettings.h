// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "ToroUserSettings.generated.h"

#define DECLARE_PROPERTY_FUNC(Type, Name) \
	void Set##Name(const Type InValue); \
	Type Get##Name() const { return Name; }

#define DECLARE_PROPERTY_FUNC_CLAMPED(Type, Name, Min, Max) \
	void Set##Name(const Type InValue); \
	Type Get##Name() const { return FMath::Clamp(Name, Min, Max); }

#define DEFINE_PROPERTY_FUNC(Type, Name, Update) \
	void UToroUserSettings::Set##Name(const Type InValue) { \
		if (Name != InValue) { \
			Name = InValue; \
			Update \
		} \
	}

#define DEFINE_PROPERTY_FUNC_CLAMPED(Type, Name, Min, Max, Update) \
	void UToroUserSettings::Set##Name(const Type InValue) { \
		if (Name != InValue) { \
			Name = FMath::Clamp(InValue, Min, Max); \
			Update \
		} \
	}

UENUM(BlueprintInternalUseOnly)
enum class ESettingApplyType : uint8
{
	Developer,
	Difficulty,
	Dynamic,
	Manual,
	UI
};

USTRUCT(BlueprintInternalUseOnly)
struct FMouseInversion
{
	GENERATED_BODY()
	
	UPROPERTY() bool X;
	UPROPERTY() bool Y;

	FMouseInversion(): X(false), Y(false) {}
	TPair<bool, bool> AsPair() { return {X, Y}; }

	static const FMouseInversion Disabled;
};

UCLASS()
class TORORUNTIME_API UToroUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UToroUserSettings();

	static UToroUserSettings* Get()
	{
		return GEngine ? Cast<UToroUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
	}

	static float GetAverageMS();
	static float GetAverageFPS();
	static void UpdateResolutions();
	static FIntPoint GetFullscreenResolution() { return FullscreenRes; }
	static TArray<FIntPoint> GetSupportedResolutions() { return SupportedResolutions; }

	bool InitializeSettings(UGameInstance* GI);
	void SetAdjustedFullscreenMode();

	void AutoAdjustScalability();
	TArray<uint8>& GetAutoScalability() { return AutoScalability; }

	void SetOverallQuality(const uint8 InValue);
	uint8 GetOverallQuality() const;

	void SetAudioVolume(const ESoundClassType InType, const uint8 InVolume);
	uint8 GetAudioVolume(const ESoundClassType InType) const
	{
		return FMath::Clamp(AudioVolume.FindRef(InType), 0, 150);
	}

	void SetSensitivityX(const float InValue);
	void SetSensitivityY(const float InValue);
	FVector2D GetRawSensitivity() const;
	FVector2D GetSensitivity() const;

	void SetInvertMouseX(const bool bInvert) { InvertMouse.X = bInvert; }
	void SetInvertMouseY(const bool bInvert) { InvertMouse.Y = bInvert; }
	const FMouseInversion& GetInvertMouse() const { return InvertMouse; }

	DECLARE_PROPERTY_FUNC(bool, ShowFPS)
	DECLARE_PROPERTY_FUNC(bool, DeveloperMode)
	DECLARE_PROPERTY_FUNC(EGameDifficulty, Difficulty)

	DECLARE_PROPERTY_FUNC(bool, SmoothCamera)

	DECLARE_PROPERTY_FUNC(EColorBlindMode, ColorBlindMode)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, ColorBlindIntensity, 0, 10)

	DECLARE_PROPERTY_FUNC(bool, Borderless)

	DECLARE_PROPERTY_FUNC(bool, FancyBloom)
	DECLARE_PROPERTY_FUNC(bool, SSFogScattering)
	DECLARE_PROPERTY_FUNC_CLAMPED(float, Gamma, 0.5f, 5.0f)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, Brightness, 10, 200)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, MotionBlur, 0, 3)

	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, LumenGI, 0, 3)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, LumenReflection, 0, 3)
	DECLARE_PROPERTY_FUNC(bool, HitLighting)

	DECLARE_PROPERTY_FUNC(EImageFidelityMode, ImageFidelity)

	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, FSRQuality, 0, 4)
	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, FSRSharpness, 0, 100)
	DECLARE_PROPERTY_FUNC(bool, FSRFrameGen)

	DECLARE_PROPERTY_FUNC_CLAMPED(uint8, XeSSQuality, 0, 6)
	DECLARE_PROPERTY_FUNC(bool, XeSSFrameGen)
	DECLARE_PROPERTY_FUNC(bool, XeSSLowLatency)

	DECLARE_MULTICAST_DELEGATE_OneParam(FUserSettingsDelegate, const ESettingApplyType)
	FUserSettingsDelegate OnSettingsUpdated;

protected:

	static inline FIntPoint FullscreenRes = {1920, 1080};
	static inline TArray<FIntPoint> SupportedResolutions = {};
	TObjectPtr<UGameInstance> GameInstance;

	void ApplyColorBlind() const;
	void ApplyScreenGamma() const;
	void ApplySSFogScattering() const;
	void ApplyImageFidelity() const;
	void ApplyAudioVolume() const;

	void ApplyFSR() const;
	void ApplyXeSS() const;

	virtual void SetToDefaults() override;
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	virtual UWorld* GetWorld() const override;

	UPROPERTY(Config) bool bInitialized;

	/* General */
	UPROPERTY(Config) bool ShowFPS;
	UPROPERTY(Config) bool DeveloperMode;
	UPROPERTY(Config) EGameDifficulty Difficulty;

	// Camera
	UPROPERTY(Config) bool SmoothCamera;
	UPROPERTY(Config) FVector2D Sensitivity;
	UPROPERTY(Config) FMouseInversion InvertMouse;

	// Accessibility
	UPROPERTY(Config) EColorBlindMode ColorBlindMode;
	UPROPERTY(Config) uint8 ColorBlindIntensity;

	/* Graphics */
	UPROPERTY(Config) bool Borderless;

	// Visuals
	UPROPERTY(Config) bool FancyBloom;
	UPROPERTY(Config) bool SSFogScattering;
	UPROPERTY(Config) float Gamma;
	UPROPERTY(Config) uint8 Brightness;
	UPROPERTY(Config) uint8 MotionBlur;

	// Lumen
	UPROPERTY(Config) uint8 LumenGI;
	UPROPERTY(Config) uint8 LumenReflection;
	UPROPERTY(Config) bool HitLighting;

	// Image Fidelity
	UPROPERTY(Config) EImageFidelityMode ImageFidelity;

	// FSR 3
	UPROPERTY(Config) uint8 FSRQuality;
	UPROPERTY(Config) uint8 FSRSharpness;
	UPROPERTY(Config) bool FSRFrameGen;
	
	// XeSS
	UPROPERTY(Config) uint8 XeSSQuality;
	UPROPERTY(Config) bool XeSSFrameGen;
	UPROPERTY(Config) bool XeSSLowLatency;
	
	/* Cached auto-set scalability */
	UPROPERTY(Config) TArray<uint8> AutoScalability{3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	
	/* Audio */
	UPROPERTY(Config) TMap<ESoundClassType, uint8> AudioVolume;
};
