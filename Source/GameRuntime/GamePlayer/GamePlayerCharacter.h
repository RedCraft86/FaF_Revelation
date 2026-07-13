// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "NativeGameplayTags.h"
#include "GamePlayerController.h"
#include "DataTypes/PlayerTypes.h"
#include "DataTypes/CachedGetter.h"
#include "DataTypes/NumericStatTypes.h"
#include "Inventory/InventoryManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "GamePlayerCharacter.generated.h"

struct FInputActionValue;
enum class EUserSettingApplyType : uint8;
class UToroGameUserSettings;
class UInteractionManager;
class UInputAction;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Char_Player)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_LockFlag)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_LockFlag_Cinematic)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_LockFlag_Loading)

UCLASS(Abstract, Blueprintable, BlueprintType)
class AGamePlayerCharacter final : public AToroPlayerCharacter
{
	GENERATED_BODY()

	friend class AGamePlayerController;

public:

	AGamePlayerCharacter();

	UFUNCTION(BlueprintCallable, Category = Player)
		void OverrideControlFlags(const int32 InFlags);

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetControlFlag(const EPlayerControlFlags InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		void UnsetControlFlag(const EPlayerControlFlags InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasControlFlag(const EPlayerControlFlags InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetStateFlag(const EPlayerStateFlags InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		void UnsetStateFlag(const EPlayerStateFlags InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasStateFlag(const EPlayerStateFlags InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLockFlag(UPARAM(meta=(Categories="LockFlag")) const FGameplayTag InFlag);

	UFUNCTION(BlueprintCallable, Category = Player)
		void UnsetLockFlag(UPARAM(meta=(Categories="LockFlag")) const FGameplayTag InFlag);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasLockFlag(UPARAM(meta=(Categories="LockFlag")) const FGameplayTag InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLockOnActor(const AActor* Target);

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLockOnComponent(const USceneComponent* Target);

	UFUNCTION(BlueprintCallable, Category = Player)
		void ClearLockOn();

	UFUNCTION(BlueprintCallable, Category = Player)
		void ResetStates(const bool bClearModifiers = true);

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetRunState(const bool bInState);

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetCrouchState(const bool bInState);

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsSneaking() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLeanState(const EPlayerLeanState InState);

	UFUNCTION(BlueprintPure, Category = Player)
		EPlayerLeanState GetLeanState() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetStaminaEnabled(bool bInEnabled);

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsStaminaEnabled() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void RefillStamina();

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetHidingSpot(AActor* InSpot);

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetOperatingTarget(AActor* InTarget);

	UFUNCTION(BlueprintCallable, Category = Player)
		bool TryKillPlayer(const FGameplayTag& FromEnemy);

	bool IsLocked() const;
	bool IsKillLocked() const;

	const FGameplayTag& GetKillEnemy() const { return KillEnemy; }
	USceneComponent* GetEquipmentRoot() const { return EquipmentRoot; }
	UInteractionManager* GetInteractionManager() const { return Interaction; }

	virtual FVector GetFocusPoint_Implementation() const override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;
	virtual bool GetLookPoint_Implementation(FVector& Location) const override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FStaminaUpdate, const bool bEnabled, const FPlayerStaminaInfo&)
	FStaminaUpdate OnStaminaUpdate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnKilled, const FGameplayTag&)
	FOnKilled OnKilled;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<class UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<class USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UInteractionManager> Interaction;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/GameRuntime.EPlayerControlFlags"))
		int32 ControlFlags;

	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/GameRuntime.EPlayerStateFlags"))
		int32 StateFlags;

	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (Categories = "LockFlag"))
		FGameplayTagContainer LockFlags;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		float InteractDistance;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float LockOnSpeed;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D LeanOffsets;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 100.0f, UIMin = 100.0f))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D StrafeOffsets;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerMovementInfo RunConfig;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerStaminaInfo Stamina;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerMovementInfo SneakConfig;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		FVector2D CrouchHeights;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FPlayerFootstepInfo Footsteps;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, TObjectPtr<UInputAction>> InputActions;

	bool bTeleporting;
	bool bUsingStamina;
	FGameplayTag KillEnemy;
	EPlayerLeanState LeanState;
	TWeakObjectPtr<AActor> HidingSpot;
	TWeakObjectPtr<AActor> OperatingTarget;
	TWeakObjectPtr<const USceneComponent> LockOnTarget;
	FNumericModifierList<float> FOVModifiers;
	FNumericValueInterp<float> FOVInterp;
	FNumericModifierList<FVector2D> CamOffsets;
	FNumericValueInterp<FVector2D> CamOffsetInterp;
	FNumericModifierList<float> MoveMultipliers;
	FNumericModifierList<FVector2D> TurnMultipliers;
	FNumericValueInterp<float> CrouchInterp;

	FTimerHandle FootstepTimer;
	FTimerHandle WallDetectTimer;

	TCachedGetter<AGamePlayerController> PlayerController {[this]()
	{
		return AGamePlayerController::Get<AGamePlayerController>(this);
	}};

	TCachedGetter<UInventoryManager> InventoryManager {[this]()
	{
		return UInventoryManager::Get(this);
	}};

	bool IsStandingBlocked() const;
	AActor* HandleInteraction() const;
	bool IsLeaningBlocked(const int32 Dir) const;
	UE5Coro::TCoroutine<> AsyncTeleport(const FVector& InLocation, const FRotator& InRotation);

	void TickFootstep();
	void TickWallDetect();
	void OnUserSettings(const UToroGameUserSettings* Settings, const EUserSettingApplyType ApplyType);
	void OnCinematic(const bool bCinematicMode);

	virtual void BeginPlay() override;
	virtual void SlowTick() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InputBinding_Pause(const FInputActionValue& InValue);
	void InputBinding_Back(const FInputActionValue& InValue);
	void InputBinding_Turn(const FInputActionValue& InValue);
	void InputBinding_Move(const FInputActionValue& InValue);
	void InputBinding_Run(const FInputActionValue& InValue);
	void InputBinding_Crouch(const FInputActionValue& InValue);
	void InputBinding_Lean(const FInputActionValue& InValue);
	void InputBinding_Interact(const FInputActionValue& InValue);
	void InputBinding_Equipment(const FInputActionValue& InValue);
};
