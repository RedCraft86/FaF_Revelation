// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/UserSettingTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "ToroUserSettings.generated.h"

#define DECLARE_PROPERTY_FUNCTIONS(Type, Name) \
	void Set##Name(const Type InValue); \
	Type Get##Name() const { return Name; } \

#define DECLARE_PROPERTY_FUNCTIONS_CLAMPED(Type, Name, Min, Max) \
	void Set##Name(const Type InValue); \
	Type Get##Name() const { return FMath::Clamp(Name, Min, Max); } \

#define DEFINE_PROPERTY_SETTER(Type, Name, Code) \
	void UToroUserSettings::Set##Name(const Type InValue) \
		Code \

#define DECLARE_CONVERTABLE_FUNCTIONS(Type, Name, AltType, AltTypeName) \
	DECLARE_PROPERTY_FUNCTIONS(Type, Name) \
	void Set##Name##AltTypeName(const AltType InValue) { Set##Name(static_cast<Type>(InValue)); } \
	AltType Get##Name##AltTypeName() const { return static_cast<AltType>(Name); }

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroUserSettings final : public UGameUserSettings
{
	GENERATED_BODY()

public:

	static UToroUserSettings* Get() { return Cast<UToroUserSettings>(GEngine->GameUserSettings); }

	DECLARE_PROPERTY_FUNCTIONS(bool, ShowFPS)
	DECLARE_PROPERTY_FUNCTIONS(FString, Username)
	void SetAudioVolume(const ESoundClassType InType, const uint8 InVolume);
	uint8 GetAudioVolume(const ESoundClassType InType) const
	{
		return FMath::Clamp(AudioVolume.FindRef(InType), 25, 150);
	}
	
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, FieldOfView, 0, 50)
	void SetFieldOfViewUI(const int32 InFieldOfView)
	{
		SetFieldOfView(FMath::Clamp(InFieldOfView + 25, 0, 50));
	}
	int32 GetFieldOfViewUI() const { return GetFieldOfView() - 25; }
	
	DECLARE_PROPERTY_FUNCTIONS(bool, SmoothCamera)
	DECLARE_PROPERTY_FUNCTIONS(float, SensitivityX)
	DECLARE_PROPERTY_FUNCTIONS(float, SensitivityY)
	
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(float, Gamma, 1.0f, 5.0f)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, Brightness, 0, 100)
	DECLARE_PROPERTY_FUNCTIONS(bool, FancyBloom)
	DECLARE_PROPERTY_FUNCTIONS(bool, ScreenSpaceFogScattering)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, MotionBlurAmount, 0, 3)

	DECLARE_PROPERTY_FUNCTIONS(bool, LumenGI)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, LumenGIQuality, 0, 3)
	
	DECLARE_PROPERTY_FUNCTIONS(bool, LumenReflections)
	DECLARE_PROPERTY_FUNCTIONS(bool, HitLightingReflections)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, LumenReflectionQuality, 0, 3)
	
	DECLARE_CONVERTABLE_FUNCTIONS(EColorBlindMode, ColorBlindMode, uint8, Int)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, ColorBlindIntensity, 0, 10)
	
	DECLARE_PROPERTY_FUNCTIONS(bool, RTXDynamicVibrance)
	DECLARE_PROPERTY_FUNCTIONS(float, DynamicVibranceIntensity)
	DECLARE_PROPERTY_FUNCTIONS(float, DynamicVibranceSaturation)
	
	DECLARE_CONVERTABLE_FUNCTIONS(EImageFidelityMode, ImageFidelityMode, uint8, Int)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, FXAADithering, 0, 4)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, TAAUpsampling, 0, 2)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, TSRResolution, 25, 200)
	
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, SMAAQuality, 0, 3)
	DECLARE_PROPERTY_FUNCTIONS_CLAMPED(uint8, SMAAEdgeMode, 0, 3)

	DECLARE_PROPERTY_FUNCTIONS(uint8, DLSSQuality)
	DECLARE_PROPERTY_FUNCTIONS(float, DLSSSharpness)
	DECLARE_PROPERTY_FUNCTIONS(bool, DLSSRayReconstruction)
	DECLARE_PROPERTY_FUNCTIONS(bool, DLSSFrameGeneration)
	DECLARE_PROPERTY_FUNCTIONS(uint8, NvidiaReflex)

	DECLARE_PROPERTY_FUNCTIONS(uint8, FSRQuality)
	DECLARE_PROPERTY_FUNCTIONS(float, FSRSharpness)
	DECLARE_PROPERTY_FUNCTIONS(bool, FSRFrameGeneration)
	
	DECLARE_PROPERTY_FUNCTIONS(uint8, XeSSQuality)
	
	DECLARE_PROPERTY_FUNCTIONS(uint8, NISQuality)
	DECLARE_PROPERTY_FUNCTIONS(float, NISSharpness)
	DECLARE_PROPERTY_FUNCTIONS(float, NISScreenPercentage)
	
private:

	void ApplyAudioSettings() const;

	virtual UWorld* GetWorld() const override;

	/* Misc */
	UPROPERTY(Config) bool ShowFPS;
	UPROPERTY(Config) FString Username;
	UPROPERTY(Config) TMap<ESoundClassType, uint8> AudioVolume;

	/* Camera */
	UPROPERTY(Config) uint8 FieldOfView;
	UPROPERTY(Config) bool SmoothCamera;
	UPROPERTY(Config) float SensitivityX;
	UPROPERTY(Config) float SensitivityY;

	/* Graphics */
	UPROPERTY(Config) float Gamma;
	UPROPERTY(Config) uint8 Brightness;
	UPROPERTY(Config) bool FancyBloom;
	UPROPERTY(Config) bool ScreenSpaceFogScattering;
	UPROPERTY(Config) uint8 MotionBlurAmount;

	// Global Illumination
	UPROPERTY(Config) bool LumenGI;
	UPROPERTY(Config) uint8 LumenGIQuality;

	// Reflections
	UPROPERTY(Config) bool LumenReflections;
	UPROPERTY(Config) bool HitLightingReflections;
	UPROPERTY(Config) uint8 LumenReflectionQuality;

	/* Color Correction */
	UPROPERTY(Config) EColorBlindMode ColorBlindMode;
	UPROPERTY(Config) uint8 ColorBlindIntensity;

	// Nvidia RTX Dynamic Vibrance
	UPROPERTY(Config) bool RTXDynamicVibrance;
	UPROPERTY(Config) float DynamicVibranceIntensity;
	UPROPERTY(Config) float DynamicVibranceSaturation;

	/* Anti-Aliasing / Upscaling */
	UPROPERTY(Config) EImageFidelityMode ImageFidelityMode;

	// Native AA / Upscaling
	UPROPERTY(Config) uint8 FXAADithering;
	UPROPERTY(Config) uint8 TAAUpsampling;
	UPROPERTY(Config) uint8 TSRResolution;

	// SMAA
	UPROPERTY(Config) uint8 SMAAQuality;
	UPROPERTY(Config) uint8 SMAAEdgeMode;

	// DLSS
	UPROPERTY(Config) uint8 DLSSQuality;
	UPROPERTY(Config) float DLSSSharpness;
	UPROPERTY(Config) bool DLSSRayReconstruction;
	UPROPERTY(Config) bool DLSSFrameGeneration;
	UPROPERTY(Config) uint8 NvidiaReflex;

	// FSR
	UPROPERTY(Config) uint8 FSRQuality;
	UPROPERTY(Config) float FSRSharpness;
	UPROPERTY(Config) bool FSRFrameGeneration; // Frame Interpolation

	// XeSS
	UPROPERTY(Config) uint8 XeSSQuality;

	// NIS
	UPROPERTY(Config) uint8 NISQuality;
	UPROPERTY(Config) float NISSharpness;
	UPROPERTY(Config) float NISScreenPercentage;
};
