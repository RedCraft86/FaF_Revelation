// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LegacyCameraShake.h"
#include "PlayerTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerControlFlags
{
	PCF_None		= 0			UMETA(Hidden),
	PCF_Locked		= 1 << 0	UMETA(DisplayName = "Locked"),

	// Ability flags
	PCF_UseStamina	= 1 << 1	UMETA(DisplayName = "Use Stamina"),
	PCF_CanPause	= 1 << 2	UMETA(DisplayName = "Can Pause"),
	PCF_CanTurn		= 1 << 3	UMETA(DisplayName = "Can Turn"),
	PCF_CanMove		= 1 << 4	UMETA(DisplayName = "Can Walk"),
	PCF_CanRun		= 1 << 5	UMETA(DisplayName = "Can Run"),
	PCF_CanCrouch	= 1 << 6	UMETA(DisplayName = "Can Crouch"),
	PCF_CanLean		= 1 << 7	UMETA(DisplayName = "Can Lean"),
	PCF_CanInteract	= 1 << 8	UMETA(DisplayName = "Can Interact"),
	PCF_CanHide		= 1 << 9	UMETA(DisplayName = "Can Hide")
};
ENUM_CLASS_FLAGS(EPlayerControlFlags);
ENUM_RANGE_BY_FIRST_AND_LAST(EPlayerControlFlags, PCF_Locked, PCF_CanHide);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerStateFlags
{
	PSF_None		= 0			UMETA(Hidden),
	PSF_Run			= 1 << 0	UMETA(DisplayName = "Running"),
	PSF_Crouch		= 1 << 1	UMETA(DisplayName = "Crouching"),
	PSF_RunLocked	= 1 << 2	UMETA(DisplayName = "Stamina Punished"),
	PSF_Inventory	= 1 << 3	UMETA(DisplayName = "Using Inventory"),
	PSF_GuideBook	= 1 << 4	UMETA(DisplayName = "Reading Guide"),
	PSF_QuickTime	= 1 << 5	UMETA(DisplayName = "Playing QTE"),
	PSF_Inspect		= 1 << 6	UMETA(DisplayName = "Inspecting"),
	PSF_Hiding		= 1 << 7	UMETA(DisplayName = "Hiding"),
	PSF_Device		= 1 << 8	UMETA(DisplayName = "Using Device"),
	PSF_Tasking		= 1 << 9	UMETA(DisplayName = "Doing Tasks"),
};
ENUM_CLASS_FLAGS(EPlayerStateFlags);

UENUM(BlueprintType)
enum class EPlayerLeanState : uint8
{
	None,
	Left,
	Right
};

UCLASS()
class FAFREVGAME_API UCamShake_Walk : public ULegacyCameraShake
{
	GENERATED_BODY()
	UCamShake_Walk();
};

UCLASS()
class FAFREVGAME_API UCamShake_Run : public ULegacyCameraShake
{
	GENERATED_BODY()
	UCamShake_Run();
};

namespace Player
{
	namespace Inputs
	{
		inline static FName Pause			= TEXT("Pause");
		inline static FName Back			= TEXT("Back");
		inline static FName Turn			= TEXT("Turn");
		inline static FName Move			= TEXT("Move");
		inline static FName Run				= TEXT("Run");
		inline static FName Crouch			= TEXT("Crouch");
		inline static FName Lean			= TEXT("Lean");
		inline static FName Inventory		= TEXT("Inventory");
		inline static FName HideQuests		= TEXT("HideQuests");
		inline static FName Interact		= TEXT("Interact");
		inline static FName Equipment		= TEXT("Equipment");

		inline static TSet All = { Pause, Back, Turn, Move, Run, Crouch, Lean,
			Inventory, HideQuests, Interact, Equipment };
	}

	namespace Keys
	{
		inline static FName Running		= TEXT("Internal_Run");
		inline static FName Crouching	= TEXT("Internal_Crouch");
		inline static FName Inspecting	= TEXT("Internal_Inspect");
	}

	static float LeanToFloat(const EPlayerLeanState& State)
	{
		switch (State)
		{
		case EPlayerLeanState::Left: return -1.0f;
		case EPlayerLeanState::Right: return 1.0f;
		default: return 0.0f;
		}
	}

	inline static int32 DefaultControls = PCF_UseStamina | PCF_CanPause | PCF_CanTurn | PCF_CanMove
		| PCF_CanRun | PCF_CanCrouch | PCF_CanLean | PCF_CanInteract | PCF_CanHide;

	inline static int32 LockingStates = PSF_Inventory | PSF_GuideBook
		| PSF_QuickTime | PSF_Inspect | PSF_Hiding | PSF_Device | PSF_Tasking;
}