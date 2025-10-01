// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "ToroUserSettings.generated.h"

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

	bool InitializeSettings();

	// TODO

protected:

	UPROPERTY(Config) bool bInitialized;

	/* General */
	UPROPERTY(Config) bool ShowFPS;
	UPROPERTY(Config) bool DeveloperMode;
	UPROPERTY(Config) EGameDifficulty Difficulty;

	// Camera
	UPROPERTY(Config) bool SmoothCamera;
	UPROPERTY(Config) FVector2D Sensitivity;

	// Accessibility
	UPROPERTY(Config) EColorBlindMode ColorBlindMode;
	UPROPERTY(Config) uint8 ColorBlindIntensity;

	/* Graphics */
	UPROPERTY(Config) bool Borderless;

	// Visuals
	UPROPERTY(Config) float Gamma;
	UPROPERTY(Config) uint8 Brightness;
	UPROPERTY(Config) bool FancyBloom;
	UPROPERTY(Config) bool ScreenSpaceFogScattering;
	UPROPERTY(Config) uint8 MotionBlur;

	// Lumen
	UPROPERTY(Config) uint8 LumenGI;
	UPROPERTY(Config) uint8 LumenReflections;
	UPROPERTY(Config) bool HitLightingReflections;

	// Image Fidelity
	UPROPERTY(Config) EImageFidelityMode ImageFidelity;
	UPROPERTY(Config) uint8 TSRResolution;

	// FSR 3
	UPROPERTY(Config) uint8 FSRQuality;
	UPROPERTY(Config) float FSRSharpness;
	UPROPERTY(Config) bool FSRFrameInterp;
	
	// XeSS
	UPROPERTY(Config) uint8 XeSSQuality;
	UPROPERTY(Config) bool XeSSFrameGen;
	UPROPERTY(Config) bool XeSSLowLatency;
	
	/* Cached auto-set scalability */
	UPROPERTY(Config) TArray<uint8> AutoScalability{3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	
	/* Audio */
	UPROPERTY(Config) TMap<ESoundClassType, uint8> AudioVolume;
};
