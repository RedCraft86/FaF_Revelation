// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameSettingTypes.generated.h"

UENUM(BlueprintType, DisplayName = "Color Blind Mode")
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

UENUM(BlueprintType, DisplayName = "Visual Fidelity Mode")
enum class EVisualFidelityMode : uint8
{
	// Fast Approximate Anti-Aliasing
	FXAA,

	// Temporal Anti-Aliasing
	TAA,

	// Temporal Super-Resolution
	TSR,

	// AMD FidelityFX Super Resolution 3.1
	FSR3,

	// Nvidia Deep Learning Super Sampling
	DLSS,

	// Intel Xe Super Sampling
	XeSS
};