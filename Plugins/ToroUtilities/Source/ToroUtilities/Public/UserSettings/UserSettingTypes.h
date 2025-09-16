// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.generated.h"

UENUM(BlueprintType)
enum class ESettingPerformance : uint8
{
	None,
	Low,
	Medium,
	High
};

UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard
};

UENUM(BlueprintType)
enum class EColorBlindMode : uint8
{
	None,
	
	// Deuteranope (Red-Green Weak/Blind)
	Deuteranope,
	
	// Protanope (Red Weak/Blind)
	Protanope,
	
	// Tritanope (Blue-Yellow Weak/Blind)
	Tritanope
};

UENUM(BlueprintType)
enum class ESoundClassType : uint8
{
	Master	UMETA(DisplayName = "Master"),
	Music	UMETA(DisplayName = "Music"),
	SoundFX UMETA(DisplayName = "SFX"),
	Voice	UMETA(DisplayName = "Voice"),
	MAX		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESoundClassType, ESoundClassType::MAX)

UENUM(BlueprintType)
enum class EImageFidelityMode : uint8
{
	// No Anti-Aliasing or Upscaling
	None,
	
	// Fast Approximate Anti-Aliasing
	FXAA,

	// Temporal Anti-Aliasing
	TAA,

	// Temporal Super-Resolution (Upscaler)
	TSR,

	// FidelityFX™ Super Resolution 3.1 (Upscaler)
	FSR
};