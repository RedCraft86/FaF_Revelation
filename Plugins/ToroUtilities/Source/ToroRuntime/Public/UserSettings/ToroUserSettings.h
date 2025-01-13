// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "ToroUserSettings.generated.h"

UENUM(BlueprintType)
enum class EColorBlindMode : uint8
{
	None,
	
	// Deuteranope (red-green weak/blind)
	Deuteranope,
	
	// Protanope (red weak/blind)
	Protanope,
	
	// Tritanope (blue-yellow weak / bind)
	Tritanope
};

UENUM(BlueprintType)
enum class ESoundType : uint8
{
	Master,
	Music,
	SoundFX,
	Voice
};

UENUM(BlueprintType)
enum class EImageFidelityMode : uint8
{
	// No Anti-Aliasing or Upscaling
	None,
	
	// Fast Approximate Anti-Aliasing
	FXAA,

	// Temporal Anti-Aliasing
	TAA,

	// Enhanced Subpixel Morphological Anti-Aliasing
	SMAA,

	// Temporal Super-Resolution (Upscaler)
	TSR,

	// Nvidia Deep Learning Super Sampling (Upscaler)
	DLSS,

	// FidelityFX™ Super Resolution 3.1 (Upscaler)
	FSR,

	// Intel® Xe Super Sampling (Upscaler)
	XeSS,
	
	// Nvidia Image Scaling (Upscaler)
	NIS
};

#define DECLARE_PROPERTY_FUNCTIONS(Type, Name) \
		void Set##Name(const Type In##Name); \
		Type Get##Name() const { return Name; } \

#define DEFINE_PROPERTY_SETTER(Type, Name, Code) \
	void UToroUserSettings::Set##Name(const Type In##Name) \
		Code \

#define DECLARE_CONVERTABLE_FUNCTIONS(Type, Name, AltType, AltTypeName) \
	DECLARE_PROPERTY_FUNCTIONS(Type, Name) \
	void Set##Name##AltTypeName(const AltType In##Name) { Set##Name(static_cast<Type>(In##Name)); } \
	AltType Get##Name##AltTypeName() const { return static_cast<AltType>(Name); }

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroUserSettings final : public UGameUserSettings
{
	GENERATED_BODY()

public:

	static UToroUserSettings* Get() { return Cast<UToroUserSettings>(GEngine->GameUserSettings); }

	DECLARE_PROPERTY_FUNCTIONS(bool, ShowFPS)
	DECLARE_PROPERTY_FUNCTIONS(FString, Username)
	
	DECLARE_PROPERTY_FUNCTIONS(uint8, FieldOfView)
	DECLARE_PROPERTY_FUNCTIONS(bool, SmoothCamera)
	DECLARE_PROPERTY_FUNCTIONS(float, SensitivityX)
	DECLARE_PROPERTY_FUNCTIONS(float, SensitivityY)
	
	DECLARE_PROPERTY_FUNCTIONS(float, Gamma)
	DECLARE_PROPERTY_FUNCTIONS(uint8, Brightness)
	DECLARE_PROPERTY_FUNCTIONS(bool, FancyBloom)
	DECLARE_PROPERTY_FUNCTIONS(bool, ScreenSpaceFogScattering)
	DECLARE_PROPERTY_FUNCTIONS(uint8, MotionBlurAmount)

	DECLARE_PROPERTY_FUNCTIONS(bool, LumenGI)
	DECLARE_PROPERTY_FUNCTIONS(uint8, LumenGIQuality)
	
	DECLARE_PROPERTY_FUNCTIONS(bool, ReflectionMethod)
	DECLARE_PROPERTY_FUNCTIONS(uint8, LumenReflectionMode)
	DECLARE_PROPERTY_FUNCTIONS(uint8, LumenReflectionQuality)
	
	DECLARE_CONVERTABLE_FUNCTIONS(EColorBlindMode, ColorBlindMode, uint8, Int)
	DECLARE_PROPERTY_FUNCTIONS(uint8, ColorBlindIntensity)
	
	DECLARE_PROPERTY_FUNCTIONS(bool, RTXDynamicVibrance)
	DECLARE_PROPERTY_FUNCTIONS(float, DynamicVibranceIntensity)
	DECLARE_PROPERTY_FUNCTIONS(float, DynamicVibranceSaturation)
	
	DECLARE_CONVERTABLE_FUNCTIONS(EImageFidelityMode, ImageFidelityMode, uint8, Int)
	DECLARE_PROPERTY_FUNCTIONS(uint8, FXAADithering)
	DECLARE_PROPERTY_FUNCTIONS(uint8, TAAUpsampling)
	DECLARE_PROPERTY_FUNCTIONS(uint8, TSRResolution)
	
	DECLARE_PROPERTY_FUNCTIONS(uint8, SMAAQuality)
	DECLARE_PROPERTY_FUNCTIONS(uint8, SMAAEdgeDetection)

	DECLARE_PROPERTY_FUNCTIONS(uint8, DLSSQuality)
	DECLARE_PROPERTY_FUNCTIONS(float, DLSSSharpness)
	DECLARE_PROPERTY_FUNCTIONS(bool, DLSSRayReconstruction)
	DECLARE_PROPERTY_FUNCTIONS(bool, DLSSFrameGeneration)
	DECLARE_PROPERTY_FUNCTIONS(uint8, NvidiaReflex)

	DECLARE_PROPERTY_FUNCTIONS(uint8, FSRQuality)
	DECLARE_PROPERTY_FUNCTIONS(float, FSRSharpness)
	DECLARE_PROPERTY_FUNCTIONS(bool, FSRFrameGeneration)
	DECLARE_PROPERTY_FUNCTIONS(bool, FSRAsyncFrameInterp)
	
	DECLARE_PROPERTY_FUNCTIONS(uint8, XeSSQuality)
	
	DECLARE_PROPERTY_FUNCTIONS(uint8, NISQuality)
	DECLARE_PROPERTY_FUNCTIONS(float, NISSharpness)
	DECLARE_PROPERTY_FUNCTIONS(float, NISScreenPercentage)
	
private:

	/* Misc */
	UPROPERTY(Config) bool ShowFPS;
	UPROPERTY(Config) FString Username;
	UPROPERTY(Config) TMap<ESoundType, uint8> SoundTypeToVolume; // float type 0 -> 100 (100 default)

	/* Camera */
	UPROPERTY(Config) uint8 FieldOfView; // float type -25 -> +25 [0 -> 50] (25 default)
	UPROPERTY(Config) bool SmoothCamera;
	UPROPERTY(Config) float SensitivityX;
	UPROPERTY(Config) float SensitivityY;

	/* Graphics */
	UPROPERTY(Config) float Gamma;
	UPROPERTY(Config) uint8 Brightness;  // float type 25 -> 200 (100 default)
	UPROPERTY(Config) bool FancyBloom;
	UPROPERTY(Config) bool ScreenSpaceFogScattering;
	UPROPERTY(Config) uint8 MotionBlurAmount;

	// Global Illumination
	UPROPERTY(Config) bool LumenGI;
	UPROPERTY(Config) uint8 LumenGIQuality;

	// Reflections
	UPROPERTY(Config) uint8 ReflectionMethod;
	UPROPERTY(Config) uint8 LumenReflectionMode;
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
	UPROPERTY(Config) uint8 TSRResolution; // float type 50 -> 200 (100 default)

	// SMAA
	UPROPERTY(Config) uint8 SMAAQuality;
	UPROPERTY(Config) uint8 SMAAEdgeDetection;

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
	UPROPERTY(Config) bool FSRAsyncFrameInterp; // Default true (Maybe this is why there's latency?)

	// XeSS
	UPROPERTY(Config) uint8 XeSSQuality;

	// NIS
	UPROPERTY(Config) uint8 NISQuality;
	UPROPERTY(Config) float NISSharpness;
	UPROPERTY(Config) float NISScreenPercentage;
};
