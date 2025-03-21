// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "InputActionValue.h"
#include "LegacyCameraShake.h"
#include "NativeGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "DataTypes/FootstepTypes.h"
#include "PlayerTypes.generated.h"

#define LockFlag(Flag) FGameplayTag::RequestGameplayTag("PlayerLock."#Flag).GetTagName()

UE_DECLARE_GAMEPLAY_TAG_CHILD(MainMenu, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(Startup, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(Loading, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(Jumpscare, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(Hiding, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(Device, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(Guide, PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_CHILD(QTE, PlayerLock);

#define BIND_INPUT_ACTION(Component, Event, Action) \
	if (const UInputAction* IA_##Action = InputActions.FindRef(Player::InputActions::Action)) \
	{ Component->BindAction(IA_##Action, ETriggerEvent::Event, this, &ThisClass::InputBinding_##Action); }

#define INPUT_BINDING_FUNCTIONS() \
	void InputBinding_Pause(const FInputActionValue& InValue); \
	void InputBinding_Turn(const FInputActionValue& InValue); \
	void InputBinding_Move(const FInputActionValue& InValue); \
	void InputBinding_Run(const FInputActionValue& InValue); \
	void InputBinding_Crouch(const FInputActionValue& InValue); \
	void InputBinding_Lean(const FInputActionValue& InValue); \
	void InputBinding_Inventory(const FInputActionValue& InValue); \
	void InputBinding_HideQuests(const FInputActionValue& InValue); \
	void InputBinding_Interact(const FInputActionValue& InValue); \
	void InputBinding_Equipment(const FInputActionValue& InValue); \
	void InputBinding_EquipmentAlt(const FInputActionValue& InValue);

#define INPUT_EVENT_BINDS(Component) \
	BIND_INPUT_ACTION(Component, Started, Pause) \
	BIND_INPUT_ACTION(Component, Triggered, Turn) \
	BIND_INPUT_ACTION(Component, Triggered, Move) \
	BIND_INPUT_ACTION(Component, Completed, Move) \
	BIND_INPUT_ACTION(Component, Canceled, Move) \
	BIND_INPUT_ACTION(Component, Started, Run) \
	BIND_INPUT_ACTION(Component, Completed, Run) \
	BIND_INPUT_ACTION(Component, Canceled, Run) \
	BIND_INPUT_ACTION(Component, Started, Crouch) \
	BIND_INPUT_ACTION(Component, Started, Lean) \
	BIND_INPUT_ACTION(Component, Completed, Lean) \
	BIND_INPUT_ACTION(Component, Canceled, Lean) \
	BIND_INPUT_ACTION(Component, Started, Inventory) \
	BIND_INPUT_ACTION(Component, Started, HideQuests) \
	BIND_INPUT_ACTION(Component, Triggered, Interact) \
	BIND_INPUT_ACTION(Component, Completed, Interact) \
	BIND_INPUT_ACTION(Component, Canceled, Interact) \
	BIND_INPUT_ACTION(Component, Started, Equipment) \
	BIND_INPUT_ACTION(Component, Started, EquipmentAlt) \
	BIND_INPUT_ACTION(Component, Completed, EquipmentAlt) \
	BIND_INPUT_ACTION(Component, Canceled, EquipmentAlt)

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

USTRUCT(BlueprintInternalUseOnly)
struct GAMERUNTIME_API FPlayerFootsteps
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps)
		TEnumAsByte<ECollisionChannel> FloorTrace;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps)
		FVector Intervals;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps, meta = (DisplayThumbnail = false))
		TObjectPtr<UFootstepDatabase> Walk;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps, meta = (DisplayThumbnail = false))
		TObjectPtr<UFootstepDatabase> Run;

	UPROPERTY(EditAnywhere, Category = PlayerFootsteps, meta = (DisplayThumbnail = false))
		TObjectPtr<UFootstepDatabase> Crouch;

	FPlayerFootsteps()
		: Volume(1.0f), FloorTrace(ECC_Visibility), Intervals(0.5f, 0.35f, 0.6f)
	{}

	float GetInterval(const bool bRun, const bool bCrouch) const
	{
		return bRun ? Intervals.Y : bCrouch ? Intervals.Z : Intervals.X;
	}

	USoundBase* GetAudio(const EPhysicalSurface Surface, const bool bRun, const bool bCrouch) const
	{
		return bRun ? Run->GetSound(Surface) : bCrouch ? Crouch->GetSound(Surface) : Walk->GetSound(Surface);
	}
};

UCLASS()
class GAMERUNTIME_API UCamShake_Walk : public ULegacyCameraShake
{
	GENERATED_BODY()
	UCamShake_Walk();
};

UCLASS()
class GAMERUNTIME_API UCamShake_Run : public ULegacyCameraShake
{
	GENERATED_BODY()
	UCamShake_Run();
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
		inline static FName RunFOV		= TEXT("Internal_RunFOV");
		inline static FName CrouchFOV	= TEXT("Internal_CrouchFOV");
	}

	namespace LockFlags
	{
		static TSet<FName> GetAll()
		{
			return {
				LockFlag(MainMenu),
				LockFlag(Startup),
				LockFlag(Loading),
				LockFlag(Cinematic),
				LockFlag(Dialogue),
				LockFlag(Jumpscare),
				LockFlag(Inventory),
				LockFlag(Hiding),
				LockFlag(Device),
				LockFlag(Guide),
				LockFlag(QTE)
			};
		}

		static TSet<FName> Immunity()
		{
			return {
				LockFlag(MainMenu),
				LockFlag(Startup),
				LockFlag(Loading),
				LockFlag(Cinematic),
				LockFlag(Dialogue),
				LockFlag(Jumpscare),
				LockFlag(Hiding),
				LockFlag(Guide)
			};
		}

		static TSet<FName> Resettable()
		{
			return {
				LockFlag(Inventory),
				LockFlag(Hiding),
				LockFlag(Device),
				LockFlag(QTE)
			};
		}
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
