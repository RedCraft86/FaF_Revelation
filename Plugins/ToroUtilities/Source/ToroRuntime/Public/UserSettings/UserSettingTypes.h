// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "UserSettingTypes.generated.h"

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
	Master,
	Music,
	SFX,
	Voice,
	MAX UMETA(Hidden)
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

	// Subpixel Morphological Anti-Aliasing (TODO 5.7 implementation)
	SMAA	UMETA(Hidden),

	// Temporal Super-Resolution (Upscaler)
	TSR,

	// FidelityFX™ Super Resolution 3.1 (Upscaler)
	FSR,
	
	// Intel® XeSS Super Resolution
	XeSS
};

inline EAntiAliasingMethod ConvertImageFidelity(const EImageFidelityMode InMode)
{
	switch (InMode)
	{
		case EImageFidelityMode::None:	return AAM_None;
		case EImageFidelityMode::FXAA:	return AAM_FXAA;
		case EImageFidelityMode::TAA:	return AAM_TemporalAA;
		// case EImageFidelityMode::SMAA:	return AAM_SMAA // TODO 5.7 implementation
		default:						return AAM_TSR; // Catch TSR, FSR, XeSS
	}
}