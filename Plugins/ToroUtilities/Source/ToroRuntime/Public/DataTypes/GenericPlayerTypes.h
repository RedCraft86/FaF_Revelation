// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GenericPlayerTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_PlayerLock);

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
#define DEFAULT_PLAYER_CONTROL_FLAGS PCF_UseStamina | PCF_CanPause | PCF_CanTurn \
	| PCF_CanMove | PCF_CanRun | PCF_CanCrouch | PCF_CanLean | PCF_CanInteract | PCF_CanHide

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerStateFlags
{
	PSF_None		= 0			UMETA(Hidden),
	PSF_Running		= 1 << 0	UMETA(DisplayName = "Running"),
	PSF_RunLocked	= 1 << 1	UMETA(DisplayName = "Stamina Punished"),
	PSF_Crouching	= 1 << 2	UMETA(DisplayName = "Crouching"),
	PSF_Hiding		= 1 << 3	UMETA(DisplayName = "Hiding"),
	PSF_Tasking		= 1 << 4	UMETA(DisplayName = "Doing Tasks"),
	PSF_LeanLeft	= 1 << 5	UMETA(DisplayName = "Leaning Left"),
	PSF_LeanRight	= 1 << 6	UMETA(DisplayName = "Leaning Right"),
};
ENUM_CLASS_FLAGS(EPlayerStateFlags);

USTRUCT(BlueprintType)
struct TORORUNTIME_API FPlayerLockFlag
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PlayerLock, meta = (Categories = "PlayerLock"))
		FGameplayTag LockTag;

	UPROPERTY(EditAnywhere, Category = PlayerLock)
		FName LockKey;

	FPlayerLockFlag() : LockTag(Tag_PlayerLock) {}
	FPlayerLockFlag(const FGameplayTag& InLock) : LockTag(InLock) {}
	FPlayerLockFlag(const FName& InLock) : LockTag(Tag_PlayerLock), LockKey(InLock) {}
	FORCEINLINE friend bool operator==(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return *A == *B; }
	FORCEINLINE friend bool operator!=(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return *A != *B; }
	FORCEINLINE friend FName operator*(const FPlayerLockFlag& InLock)
	{
		return InLock.HasValidTag() ? InLock.LockTag.GetTagName() : InLock.LockKey;
	}

	bool IsValidFlag() const { return HasValidTag() || !LockKey.IsNone(); }
	bool HasValidTag() const { return LockTag.IsValid() && LockTag != Tag_PlayerLock; }

#if WITH_EDITOR
	void ResetTag() { LockTag = Tag_PlayerLock; }
#endif
};
