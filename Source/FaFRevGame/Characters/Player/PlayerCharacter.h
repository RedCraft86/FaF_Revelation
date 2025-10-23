// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputAction.h"
#include "PlayerTypes.h"
#include "DataTypes/ToroModifiers.h"
#include "DataTypes/ToroInterpolator.h"
#include "Framework/ToroPlayerCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FAFREVGAME_API APlayerCharacter final : public AToroPlayerCharacter
{
	GENERATED_BODY()
#if WITH_EDITOR
	friend class FPlayerCharacterDetails;
#endif
public:

	APlayerCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UQTEManager> QuicktimeEvents;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 ControlFlags;

	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerStateFlags"))
		int32 StateFlags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, Units = "cm"))
		float InteractDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float LockOnSpeed;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D LeanOffsets;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> SideTrace;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 100.0f, UIMin = 100.0f))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D StrafeOffsets;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerMovementInfo RunConfig;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerStamina Stamina;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerMovementInfo SneakConfig;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		FVector2D HalfHeights;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> CeilingTrace;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FPlayerFootsteps Footsteps;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, TObjectPtr<UInputAction>> InputActions;

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

	UFUNCTION(BlueprintPure, Category = Player)
		int32 GetActivityFlags() const;

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasActivityFlag(const EPlayerActivityFlags InFlag) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void AddFieldOfViewMod(const FName Key, const float Addition);

	UFUNCTION(BlueprintCallable, Category = Player)
		void RemoveFieldOfViewMod(const FName Key);

	UFUNCTION(BlueprintCallable, Category = Player)
		bool HasFieldOfViewMod(const FName Key) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void AddCameraOffsetMod(const FName Key, const FVector2D Addition);

	UFUNCTION(BlueprintCallable, Category = Player)
		void RemoveCameraOffsetMod(const FName Key);

	UFUNCTION(BlueprintCallable, Category = Player)
		bool HasCameraOffsetMod(const FName Key) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void AddMovementSpeedMod(const FName Key, const float Multiplier);

	UFUNCTION(BlueprintCallable, Category = Player)
		void RemoveMovementSpeedMod(const FName Key);

	UFUNCTION(BlueprintCallable, Category = Player)
		bool HasMovementSpeedMod(const FName Key) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void AddSensitivityMod(const FName Key, const FVector2D Multiplier);

	UFUNCTION(BlueprintCallable, Category = Player)
		void RemoveSensitivityMod(const FName Key);

	UFUNCTION(BlueprintCallable, Category = Player)
		bool HasSensitivityMod(const FName Key) const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void ResetStates();
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetRunState(const bool bInState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsRunState() const;
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsRunning() const;
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetCrouchState(const bool bInState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsCrouchState() const;
	
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
		void SetOperatingActor(AActor* InActor);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FStaminaUpdate, const bool bEnabled, const FPlayerStamina&)
	FStaminaUpdate OnStaminaUpdate;

	bool TryJumpscare(const FGameplayTag& FromEnemy);
	const FGameplayTag& GetJumpscareEnemy() const { return JumpscareEnemy; }

	virtual bool IsKillLocked() override;
	virtual bool IsControlLocked() override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

protected:

	EPlayerLeanState LeanState;
	FGameplayTag JumpscareEnemy;

	FToroSumFloat FieldOfView;
	FToroInterpFloat InterpFOV;

	FToroSumVector2D CameraOffset;
	FToroInterpVector2D InterpCamOffset;

	FToroMultiFloat MovementMulti;
	FToroMultiVector2D Sensitivity;
	FToroInterpFloat InterpCrouch;

	TWeakObjectPtr<AActor> HidingSpot;
	TWeakObjectPtr<AActor> OperatingActor;
	TObjectPtr<class UWorldMusicManager> MusicManager;
	TObjectPtr<class UTutorialManager> TutorialManager;
	TObjectPtr<class UInventoryManager> InventoryManager;
	TObjectPtr<class UInspectionManager> InspectionManager;
	TObjectPtr<class UNarrativeManager> NarrativeManager;

	FTimerHandle StaminaTimer;
	FTimerHandle FootstepTimer;
	FTimerHandle WallDetectTimer;

	void TickStamina();
	void TickFootstep();
	void TickWallDetect();

	bool IsStandingBlocked() const;
	bool IsLeaningBlocked(const int32 Dir) const;
	FHitResult HandleInteraction() const;

	virtual void OnCinematic(AActor* InActor) override;
	virtual void OnSettingsUpdate(const ESettingApplyType ApplyType) override;
	
	virtual void SlowTick() override;
	virtual void BeginPlay() override;
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
	void InputBinding_Inventory(const FInputActionValue& InValue);
	void InputBinding_HideQuests(const FInputActionValue& InValue);
	void InputBinding_Interact(const FInputActionValue& InValue);
	void InputBinding_Equipment(const FInputActionValue& InValue);
};
