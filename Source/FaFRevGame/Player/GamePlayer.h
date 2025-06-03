// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExitInterface.h"
#include "PlayerTypes.h"
#include "DataTypes/MathTypes.h"
#include "Narrative/GameNarrative.h"
#include "Inventory/GameInventory.h"
#include "Framework/ToroPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GamePlayer.generated.h"

UCLASS()
class FAFREVGAME_API AGamePlayer final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	AGamePlayer();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UGameNarrative> Narrative;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UGameInventory> Inventory;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 ControlFlags;
	
	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerStateFlags"))
		int32 StateFlags;

	UPROPERTY(EditAnywhere, Category = FOV, meta = (UIMin = 70.0f, UIMax = 120.0f))
		FToroFloatSum FOVValue;

	UPROPERTY(EditAnywhere, Category = FOV, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float FOVSpeed;

	UPROPERTY(EditAnywhere, Category = FOV)
		float FOVRunOffset;

	UPROPERTY(EditAnywhere, Category = FOV)
		float FOVCrouchOffset;

	UPROPERTY(EditAnywhere, Category = LockOn, meta = (DisplayThumbnail = false))
		TObjectPtr<const USceneComponent> LockOnTarget;

	UPROPERTY(EditAnywhere, Category = LockOn, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float LockOnSpeed;

	UPROPERTY(EditAnywhere, Category = Sensitivity, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D Sensitivity;

	UPROPERTY(EditAnywhere, Category = Sensitivity, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FToroFloatMulti SensitivityMulti;

	UPROPERTY(EditAnywhere, Category = Leaning)
		FVector2D LeanOffsets;

	UPROPERTY(EditAnywhere, Category = Leaning, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float LeanSpeed;

	UPROPERTY(EditAnywhere, Category = Leaning)
		TEnumAsByte<ECollisionChannel> SideTrace;

	UPROPERTY(EditAnywhere, Category = Movement)
		FVector2D StrafeOffsets;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (ClampMin = 100.0f, UIMin = 100.0f))
		FToroFloatMulti WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float RunSpeedMulti;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 1.0, UIMax = 1.0))
		float SneakSpeedMulti;

	UPROPERTY(EditAnywhere, Category = Crouching, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		FVector2D CrouchHeights;

	UPROPERTY(EditAnywhere, Category = Crouching, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float CrouchSpeed;

	UPROPERTY(EditAnywhere, Category = Crouching)
		TEnumAsByte<ECollisionChannel> CeilingTrace;

	UPROPERTY(EditAnywhere, Category = Stamina, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		float MaxStamina;

	UPROPERTY(EditAnywhere, Category = Stamina, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D StaminaRates;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		FVector FSIntervals;

	UPROPERTY(EditAnywhere, Category = Footsteps)
		TEnumAsByte<ECollisionChannel> FSTrace;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> FSWalk;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> FSRun;

	UPROPERTY(EditAnywhere, Category = Footsteps, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> FSSneak;

	UPROPERTY(EditAnywhere, Category = Input, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, TObjectPtr<UInputAction>> InputActions;

private:

	UPROPERTY() FTimerHandle StaminaTimer;
	UPROPERTY() FTimerHandle FootstepTimer;
	UPROPERTY() FTimerHandle WallDetectTimer;
	
	UPROPERTY() FToroFloatInterp InterpFOV;
	UPROPERTY() FToroFloatInterp InterpCrouch;
	UPROPERTY() FToroVectorInterp InterpCamOffset;

	UPROPERTY() float Stamina;
	UPROPERTY() FVector2D CamLeanOffset;
	UPROPERTY() FVector2D CamStrafeOffset;
	UPROPERTY() EPlayerLeanState LeanState;

	UPROPERTY(Transient) TObjectPtr<UObject> HidingSpot;
	UPROPERTY(Transient) TObjectPtr<UObject> WorldDevice;
	UPROPERTY(Transient) TObjectPtr<UObject> TaskDevice;

public:

	UFUNCTION(BlueprintPure, Category = Player)
		float GetSpeed() const { return GetVelocity().Size2D(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsMoving() const { return GetSpeed() > 50.0f; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsLocked() const { return ControlFlags & PCF_Locked || !LockFlags.IsEmpty(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void ResetStates();
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void OverrideControlFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags")) const int32 InFlags);
	
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
		void SetRunState(const bool bInState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsRunning() const { return HasStateFlag(PSF_Running) && GetSpeed() > WalkSpeed.Base + 25.0f; }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetCrouchState(const bool bInState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsCrouching() const { return HasStateFlag(PSF_Crouching); }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLeanState(const EPlayerLeanState InState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		EPlayerLeanState GetLeanState() const { return LeanState; }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetStaminaPercent(const float InStamina = 1.0f);
	
	UFUNCTION(BlueprintPure, Category = Player)
		float GetStaminaPercent() const { return Stamina / MaxStamina; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsStaminaPunished() const { return HasControlFlag(PCF_UseStamina) && HasStateFlag(PSF_RunLocked); }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLockOnTarget(const USceneComponent* InComponent);

	UFUNCTION(BlueprintPure, Category = Player)
		const USceneComponent* GetLockOnTarget() const { return LockOnTarget; }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetHidingSpot(UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = Player)
		void ExitHiding() const { IExitInterface::Exit(HidingSpot); }

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetHidingSpot() const { return HidingSpot; }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetWorldDevice(UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = Player)
		void ExitWorldDevice() const { IExitInterface::Exit(WorldDevice); }

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetWorldDevice() const { return WorldDevice; }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetTaskDevice(UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = Player)
		void ExitTaskDevice() const { IExitInterface::Exit(TaskDevice); }

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetTaskDevice() const { return TaskDevice; }

	UFUNCTION(BlueprintCallable, Category = Player)
		bool TryJumpscare();

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual bool GetLookTarget_Implementation(FVector& Location) const override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;
	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

private:

	void TickStamina();
	void TickFootstep();
	void LeanWallDetect();
	bool IsStandingBlocked() const;
	bool IsLeaningBlocked(const float Direction) const;
	USoundBase* GetFootstepSound(const EPhysicalSurface Surface) const;
	float GetFootstepInterval() const;

	void OnSettingsChange(const class UToroUserSettings* InSettings);
	
	virtual void SlowTick() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	void InputBinding_Pause(const FInputActionValue& InValue);
	void InputBinding_Turn(const FInputActionValue& InValue);
	void InputBinding_Move(const FInputActionValue& InValue);
	void InputBinding_Run(const FInputActionValue& InValue);
	void InputBinding_Crouch(const FInputActionValue& InValue);
	void InputBinding_Lean(const FInputActionValue& InValue);
	void InputBinding_Inventory(const FInputActionValue& InValue);
	void InputBinding_HideQuests(const FInputActionValue& InValue);
	void InputBinding_Interact(const FInputActionValue& InValue);
	void InputBinding_Equipment(const FInputActionValue& InValue);
	void InputBinding_EquipmentAlt(const FInputActionValue& InValue);
};
