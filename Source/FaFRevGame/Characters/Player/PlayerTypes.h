// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LegacyCameraShake.h"
#include "PlayerTypes.generated.h"

/** Player capability flags determining which actions are permitted. */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerControlFlags
{
	PCF_None		= 0			UMETA(Hidden),
	PCF_UseStamina	= 1 << 0	UMETA(DisplayName = "Use Stamina"),
	PCF_CanPause	= 1 << 1	UMETA(DisplayName = "Can Pause"),
	PCF_CanTurn		= 1 << 2	UMETA(DisplayName = "Can Turn"),
	PCF_CanMove		= 1 << 3	UMETA(DisplayName = "Can Move"),
	PCF_CanRun		= 1 << 4	UMETA(DisplayName = "Can Run"),
	PCF_CanCrouch	= 1 << 5	UMETA(DisplayName = "Can Crouch"),
	PCF_CanLean		= 1 << 6	UMETA(DisplayName = "Can Lean"),
	PCF_CanInteract	= 1 << 7	UMETA(DisplayName = "Can Interact"),
	PCF_CanHide		= 1 << 8	UMETA(DisplayName = "Can Hide")
};
ENUM_CLASS_FLAGS(EPlayerControlFlags);
ENUM_RANGE_BY_FIRST_AND_LAST(EPlayerControlFlags, PCF_UseStamina, PCF_CanHide);

/** Player state flags showing the current internal state of the player. */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerStateFlags
{
	PSF_None		= 0			UMETA(Hidden),
	PSF_Move		= 1 << 0	UMETA(DisplayName = "Moving"),
	PSF_Run			= 1 << 1	UMETA(DisplayName = "Running"),
	PSF_Crouch		= 1 << 2	UMETA(DisplayName = "Crouching"),
	PSF_Leaning		= 1 << 3	UMETA(DisplayName = "Leaning"),
	PSF_Interact	= 1 << 4	UMETA(DisplayName = "Interacting")
};
ENUM_CLASS_FLAGS(EPlayerStateFlags);
ENUM_RANGE_BY_FIRST_AND_LAST(EPlayerStateFlags, PSF_Move, PSF_Leaning);

/** Player state flags showing the current player engagement with other systems. */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerActivityFlags
{
	PAF_None		= 0	UMETA(Hidden),
	PAF_Hiding		= 1 << 0	UMETA(DisplayName = "Hiding"),
	PAF_Operating	= 1 << 1	UMETA(DisplayName = "Operating"),
	PAF_Inspect		= 1 << 2	UMETA(DisplayName = "Inspecting"),
	PAF_Inventory	= 1 << 3	UMETA(DisplayName = "Inventory"),
	PAF_Tutorial	= 1 << 4	UMETA(DisplayName = "Tutorial"),
	PAF_Dialogue	= 1 << 5	UMETA(DisplayName = "Dialogue"),
	PAF_QTE			= 1 << 6	UMETA(DisplayName = "QTE")
};
ENUM_CLASS_FLAGS(EPlayerActivityFlags);
ENUM_RANGE_BY_FIRST_AND_LAST(EPlayerActivityFlags, PAF_Inspect, PAF_QTE);

UENUM(BlueprintType)
enum class EPlayerLeanState : uint8
{
	None,
	Left,
	Right
};

USTRUCT(BlueprintInternalUseOnly)
struct FPlayerMovementInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Rates, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float SpeedMultiplier;

	UPROPERTY(EditAnywhere, Category = Rates)
		float FieldOfViewOffset;

	FPlayerMovementInfo(): SpeedMultiplier(1.0f), FieldOfViewOffset(0.0f) {}
	FPlayerMovementInfo(const float InSpeedMulti, const float InFOV)
		: SpeedMultiplier(InSpeedMulti), FieldOfViewOffset(InFOV)
	{}
};

USTRUCT(BlueprintInternalUseOnly)
struct FStaminaRates
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Rates, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Depletion;

	UPROPERTY(EditAnywhere, Category = Rates, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Regeneration;

	FStaminaRates(): Depletion(1.0f), Regeneration(1.0f) {}
	FStaminaRates(const float InDepletion, const float InRegeneration)
		: Depletion(InDepletion), Regeneration(InRegeneration)
	{}
};

/**
 * Stamina is represented as a single continuous bar, but divided into two behavior zones:
 * 
 * [Empty | Reserve | Normal | Full]
 * 
 * - When stamina is above the reserve threshold, NormalRates are used.
 * - When stamina drops into the reserve zone, ReserveRates apply.
 * 
 * This design discourages prolonged chases by making recovery slower and drain faster
 * once the player enters the reserve zone. This system is less punishing than the
 * previous exhaustion system where if the player use up all stamina, they are unable
 * to run again until the bar is completely regenerated.
 */
USTRUCT(BlueprintInternalUseOnly)
struct FPlayerStamina
{
	GENERATED_BODY()

	FPlayerStamina()
		: MaxNormal(100.0f), NormalRates(1.0f, 1.0f)
		, MaxReserve(25.0f), ReserveRates(1.5f, 0.5f)
		, Stamina(100.0f), Delta(0.0f)
	{}

	FPlayerStamina(const float MaxNormal, const float MaxReserve)
		: MaxNormal(MaxNormal), NormalRates(1.0f, 1.0f)
		, MaxReserve(MaxReserve), ReserveRates(1.5f, 0.5f)
		, Stamina(100.0f), Delta(0.0f)
	{}

	float GetDelta() const { return Delta; }
	float GetMaxStamina() const { return MaxNormal + MaxReserve; }

	float GetOverallPercent() const { return Stamina / GetMaxStamina(); }
	float GetNormalPercent() const { return FMath::Clamp((Stamina - MaxReserve) / MaxNormal, 0.0f, 1.0f); }
	float GetReservePercent() const { return FMath::Clamp(Stamina / MaxReserve, 0.0f, 1.0f); }
	void SetPercent(const float InPercent)
	{
		Stamina = FMath::Lerp(0.0f, GetMaxStamina(), InPercent);
	}

	bool IsInReserve() const { return Stamina <= MaxReserve; }
	bool IsEmpty() const { return FMath::IsNearlyZero(Stamina); }
	bool IsFull() const { return FMath::IsNearlyEqual(Stamina, GetMaxStamina()); }

	void TimedTick(const int32 StateFlags);

private:

	UPROPERTY(EditAnywhere, Category = "Stamina|Normal", DisplayName = "Max")
		float MaxNormal;

	UPROPERTY(EditAnywhere, Category = "Stamina|Normal", DisplayName = "Rates")
		FStaminaRates NormalRates;

	UPROPERTY(EditAnywhere, Category = "Stamina|Reserve", DisplayName = "Max")
		float MaxReserve;

	UPROPERTY(EditAnywhere, Category = "Stamina|Reserve", DisplayName = "Rates")
		FStaminaRates ReserveRates;

	float Stamina, Delta;
};

USTRUCT(BlueprintInternalUseOnly)
struct FSurfaceFootstep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Interval;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> Sounds;

	FSurfaceFootstep(): Interval(0.0f) {}
	FSurfaceFootstep(const float InInterval): Interval(InInterval) {}
	USoundBase* GetSurfaceSound(const EPhysicalSurface Surface) const;
#if WITH_EDITOR
	void FillSlots();
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct FPlayerFootsteps
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Footsteps)
		TObjectPtr<USoundBase> DefaultSound;

	UPROPERTY(EditAnywhere, Category = Footsteps)
		FSurfaceFootstep WalkSounds;

	UPROPERTY(EditAnywhere, Category = Footsteps)
		FSurfaceFootstep RunSounds;

	UPROPERTY(EditAnywhere, Category = Footsteps)
		FSurfaceFootstep SneakSounds;

	FPlayerFootsteps() {}
	float GetFootstepInterval(const int32 StateFlags) const;
	USoundBase* GetFootstepSound(const int32 StateFlags, const EPhysicalSurface Surface) const;
#if WITH_EDITOR
	void FillSlots();
#endif
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
	inline constexpr int32 DefaultAbilities = PCF_UseStamina | PCF_CanPause | PCF_CanTurn
		| PCF_CanMove | PCF_CanRun | PCF_CanCrouch | PCF_CanLean | PCF_CanInteract | PCF_CanHide;

	inline int32 LeanToDir(const EPlayerLeanState& State)
	{
		switch (State)
		{
			case EPlayerLeanState::Left:	return -1;
			case EPlayerLeanState::Right:	return  1;
			default:						return  0;
		}
	}

	namespace Internal
	{
		inline const FName Leaning		= TEXT("Internal_Leaning");
		inline const FName Running		= TEXT("Internal_Running");
		inline const FName Crouching	= TEXT("Internal_Crouching");
	}

	namespace Inputs
	{
		inline const FName Pause		= TEXT("Pause");
		inline const FName Back			= TEXT("Back");
		inline const FName Turn			= TEXT("Turn");
		inline const FName Move			= TEXT("Move");
		inline const FName Run			= TEXT("Run");
		inline const FName Crouch		= TEXT("Crouch");
		inline const FName Lean			= TEXT("Lean");
		inline const FName Inventory	= TEXT("Inventory");
		inline const FName HideQuests	= TEXT("HideQuests");
		inline const FName Interact		= TEXT("Interact");
		inline const FName Equipment	= TEXT("Equipment");

		inline const TSet All = {
			Pause,
			Back,
			Turn,
			Move,
			Run,
			Crouch,
			Lean,
			Inventory,
			HideQuests,
			Interact,
			Equipment
		};
	}
}