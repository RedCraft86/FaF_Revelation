// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "FootstepTypes.h"
#include "NativeGameplayTags.h"
#include "GenericPlayerTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockStartup);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockLoading);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockCutscene);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockDialogue);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockJumpscare);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockInventory);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockHiding);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockDevice);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockGuide);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockQTE);

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
	PSF_Tasking		= 1 << 4	UMETA(DisplayName = "Doing Tasks")
};
ENUM_CLASS_FLAGS(EPlayerStateFlags);

UENUM(BlueprintType)
enum class EPlayerLeanState : uint8
{
	None,
	Left,
	Right
};

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

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPlayerFootsteps
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps)
		TEnumAsByte<ECollisionChannel> FloorTrace;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps)
		FVector Intervals;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps, meta = (DisplayThumbnail = false))
		TObjectPtr<UFootstepDatabase> Sounds;

	FPlayerFootsteps()
		: Volume(1.0f), FloorTrace(ECC_Visibility), Intervals(0.5f, 0.35f, 0.6f)
	{}

	float GetInterval(const bool bRun, const bool bCrouch) const
	{
		return bRun ? Intervals.Y : bCrouch ? Intervals.Z : Intervals.X;
	}

	USoundBase* GetAudio(const EPhysicalSurface Surface, const bool bRun, const bool bCrouch) const
	{
		return Sounds->GetSound(Surface, bRun, bCrouch);
	}
};

namespace Player
{
	namespace InputActions
	{
		inline static FName Pause			= TEXT("Pause");
		inline static FName Turn			= TEXT("Turn");
		inline static FName Move			= TEXT("Move");
		inline static FName Run				= TEXT("Run");
		inline static FName Crouch			= TEXT("Crouch");
		inline static FName Lean			= TEXT("Lean");
		inline static FName Inventory		= TEXT("Inventory");
		inline static FName HideQuests		= TEXT("HideQuests");
		inline static FName Interact		= TEXT("Interact");
		inline static FName Equipment		= TEXT("Equipment");
		inline static FName EquipmentAlt	= TEXT("EquipmentAlt");

		inline static TSet All = { Pause, Turn, Move, Run, Crouch, Lean,
			Inventory, HideQuests, Interact, Equipment, EquipmentAlt };
	}

	namespace InternalKeys
	{
		inline static FString RunFOV		= TEXT("Internal_RunFOV");
		inline static FString CrouchFOV		= TEXT("Internal_CrouchFOV");
	}

	namespace LockFlags
	{
		// inline static TSet<FGameplayTag> All = {
		// 	Tag_LockStartup.GetTag(),
		// 	Tag_LockLoading.GetTag(),
		// 	Tag_LockCutscene.GetTag(),
		// 	Tag_LockDialogue.GetTag(),
		// 	Tag_LockJumpscare.GetTag(),
		// 	Tag_LockInventory.GetTag(),
		// 	Tag_LockHiding.GetTag(),
		// 	Tag_LockDevice.GetTag(),
		// 	Tag_LockGuide.GetTag(),
		// 	Tag_LockQTE.GetTag()
		// };
		//
		// inline static TSet<FGameplayTag> Immunity = {
		// 	Tag_LockStartup.GetTag(),
		// 	Tag_LockLoading.GetTag(),
		// 	Tag_LockCutscene.GetTag(),
		// 	Tag_LockDialogue.GetTag(),
		// 	Tag_LockJumpscare.GetTag(),
		// 	Tag_LockHiding.GetTag(),
		// 	Tag_LockGuide.GetTag()
		// };
		//
		// inline static TSet<FGameplayTag> Resettable = {
		// 	Tag_LockInventory.GetTag(),
		// 	Tag_LockHiding.GetTag(),
		// 	Tag_LockDevice.GetTag(),
		// 	Tag_LockQTE.GetTag()
		// };
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
}
