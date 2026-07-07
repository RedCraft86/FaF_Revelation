// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Misc/EnumRange.h"
#include "Sound/SoundBase.h"
#include "Engine/EngineTypes.h"
#include "DataTypes/SimpleCooldown.h"
#include "Chaos/ChaosEngineInterface.h"
#include "PlayerTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = true))
enum EPlayerControlFlags
{
	PCF_None		= 0			UMETA(Hidden),
	PCF_CanPause	= 1 << 0	UMETA(DisplayName = "Can Pause"),
	PCF_CanTurn		= 1 << 1	UMETA(DisplayName = "Can Turn"),
	PCF_CanMove		= 1 << 2	UMETA(DisplayName = "Can Move"),
	PCF_CanRun		= 1 << 3	UMETA(DisplayName = "Can Run"),
	PCF_CanCrouch	= 1 << 4	UMETA(DisplayName = "Can Crouch"),
	PCF_CanLean		= 1 << 5	UMETA(DisplayName = "Can Lean"),
	PCF_UseStamina	= 1 << 6	UMETA(DisplayName = "Use Stamina"),
	PCF_CanInteract	= 1 << 7	UMETA(DisplayName = "Can Interact"),
	PCF_CanHide		= 1 << 8	UMETA(DisplayName = "Can Hide")
};
ENUM_CLASS_FLAGS(EPlayerControlFlags)
ENUM_RANGE_BY_VALUES(EPlayerControlFlags, PCF_CanPause, PCF_CanTurn, PCF_CanMove, 
	PCF_CanRun, PCF_CanCrouch, PCF_CanLean, PCF_UseStamina, PCF_CanInteract, PCF_CanHide)

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = true))
enum EPlayerStateFlags
{
	PSF_None		= 0			UMETA(Hidden),
	PSF_Move		= 1 << 0	UMETA(DisplayName = "Moving"),
	PSF_Run			= 1 << 1	UMETA(DisplayName = "Running"),
	PSF_Crouch		= 1 << 2	UMETA(DisplayName = "Crouching"),
	PSF_Lean		= 1 << 3	UMETA(DisplayName = "Leaning"),
	PSF_Interact	= 1 << 4	UMETA(DisplayName = "Interacting"),
	PSF_Hide		= 1 << 5	UMETA(DisplayName = "Hiding"),
	PSF_Operate		= 1 << 6	UMETA(DisplayName = "Operating"),
	PSF_Tutorial	= 1 << 7	UMETA(DisplayName = "Learning"),
	PSF_Dialogue	= 1 << 8	UMETA(DisplayName = "Talking")
};
ENUM_CLASS_FLAGS(EPlayerStateFlags)
ENUM_RANGE_BY_VALUES(EPlayerStateFlags, PSF_Move, PSF_Run, PSF_Crouch, 
	PSF_Lean, PSF_Interact, PSF_Hide, PSF_Operate, PSF_Tutorial, PSF_Dialogue)

UENUM(BlueprintType)
enum class EPlayerLeanState : uint8
{
	None,
	Left,
	Right
};

USTRUCT(BlueprintInternalUseOnly)
struct FPlayerMovementInfo final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Info)
		float FOV_Offset;

	UPROPERTY(EditAnywhere, Category = Info, meta = (ClampMin = 0.1f, UIMin = 0.1f, Units = "x"))
		float SpeedMultiplier;

	FPlayerMovementInfo(): FOV_Offset(0.0f), SpeedMultiplier(1.0f) {}
	FPlayerMovementInfo(const float InFOV, const float InSpeedMulti)
		: FOV_Offset(InFOV), SpeedMultiplier(InSpeedMulti)
	{}
};

USTRUCT(BlueprintInternalUseOnly)
struct FPlayerStaminaInfo final
{
	GENERATED_BODY()

private:

	float Amount;
	bool bPunished;

public:

	UPROPERTY(EditAnywhere, Category = Info)
		FSimpleCooldown TickInterval;

	UPROPERTY(EditAnywhere, Category = Info, meta = (ClampMin = 0.1f, UIMin = 0.1f, Units = "x"))
		float DrainRate;

	UPROPERTY(EditAnywhere, Category = Info, meta = (ClampMin = 0.1f, UIMin = 0.1f, Units = "x"))
		float GainRate;

	FPlayerStaminaInfo()
		: Amount(1.0f), bPunished(false), TickInterval(0.05f), DrainRate(0.15f), GainRate(0.1f)
	{}

	float GetStamina() const { return Amount; }
	bool IsPunished() const { return bPunished; }

	bool IsEmpty() const { return FMath::IsNearlyZero(Amount); }
	void SetPercent(const float Percent) { Amount = FMath::Clamp(Percent, 0.0f, 1.0f); }

	bool TickStamina(const float DeltaTime, const bool bRunning);
};

USTRUCT(BlueprintInternalUseOnly)
struct FPlayerFootstepInfo final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Info, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = Info, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector Intervals;

	UPROPERTY(EditAnywhere, Category = Info)
		TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, Category = Info)
		TObjectPtr<USoundBase> DefaultSound;

	UPROPERTY(EditAnywhere, Category = Info, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> SurfaceSounds;

	FPlayerFootstepInfo()
		: Volume(1.0f), Intervals(0.5f, 0.35f, 0.6f), TraceChannel(ECC_Visibility) 
	{}

	void AllocateSlots();
	float GetFootstepInterval(const int32 StateFlags) const;
	USoundBase* GetFootstepSound(const EPhysicalSurface Surface) const;
};

namespace Player
{
	inline constexpr int32 DefaultAbilities = PCF_CanPause | PCF_CanTurn | PCF_CanMove | PCF_CanRun 
		| PCF_CanCrouch | PCF_CanLean | PCF_UseStamina | PCF_CanInteract | PCF_CanHide;

	inline int32 LeanToNumber(const EPlayerLeanState& State)
	{
		switch (State)
		{
			case EPlayerLeanState::Left:	return -1;
			case EPlayerLeanState::Right:	return  1;
			default:						return  0;
		}
	}

	namespace InternalKeys
	{
		inline const FName Level		= "Internal_Level";
		inline const FName Movement		= "Internal_Movement";
		inline const FName Running		= "Internal_Running";
		inline const FName Crouching	= "Internal_Crouching";
		inline const FName Leaning		= "Internal_Leaning";
	}

	namespace InputKeys
	{
		inline const FName Pause		= "Pause";
		inline const FName Back			= "Back";
		inline const FName Turn			= "Turn";
		inline const FName Move			= "Move";
		inline const FName Run			= "Run";
		inline const FName Crouch		= "Crouch";
		inline const FName Lean			= "Lean";
		inline const FName ToggleQuests	= "ToggleQuests";
		inline const FName Interact		= "Interact";
		inline const FName Equipment	= "Equipment";

		inline const TSet All = {
			Pause,
			Back,
			Turn,
			Move,
			Run,
			Crouch,
			Lean,
			ToggleQuests,
			Interact,
			Equipment
		};
	}
}