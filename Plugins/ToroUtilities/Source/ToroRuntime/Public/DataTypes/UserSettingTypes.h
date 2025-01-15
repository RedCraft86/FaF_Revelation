﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.generated.h"

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