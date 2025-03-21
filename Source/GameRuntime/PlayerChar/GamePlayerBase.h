// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MathTypes.h"
#include "PlayerTypes.h"
#include "Camera/CameraComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Interaction/InteractionComponent.h"
#include "GamePlayerBase.generated.h"

UCLASS()
class GAMERUNTIME_API AGamePlayerBase : public AToroPlayerCharacter
{
GENERATED_BODY()

public:
	
	AGamePlayerBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionComponent> Interaction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/GameRuntime.EPlayerControlFlags"))
		int32 ControlFlags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/GameRuntime.EPlayerStateFlags"))
		int32 StateFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 100.0f, UIMax = 300.0f))
		float ReachDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatSum FieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, UIMax = 25.0f))
		float FieldOfViewSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (DisplayThumbnail = false))
		TObjectPtr<const USceneComponent> LockOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 5.0f, UIMin = 5.0f, UIMax = 25.0f))
		float LockOnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D Sensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti SensitivityMulti;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D LeanOffsets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float LeanSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TEnumAsByte<ECollisionChannel> SideTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		float SideTraceLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti MoveSpeedMulti;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, UIMax = 1000.0f))
		float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D SwayOffsets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, UIMax = 1000.0f))
		float CrouchWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = -10.0f, UIMin = -10.0f, ClampMax = 10.0f, UIMax = 10.0f))
		float CrouchFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TEnumAsByte<ECollisionChannel> CeilingTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D CrouchHeights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, UIMax = 2.0f))
		float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, UIMax = 1000.0f))
		float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = -10.0f, UIMin = -10.0f, ClampMax = 10.0f, UIMax = 10.0f))
		float RunningFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 1.0f, UIMax = 200.0f))
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti StaminaGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPlayerFootsteps Footsteps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, TObjectPtr<UInputAction>> InputActions;

	DECLARE_DELEGATE_FourParams(FStaminaUpdate, const float, const float, const float, const bool);
	FStaminaUpdate OnStaminaUpdate;
	
protected:

	UPROPERTY() FTimerHandle StaminaTimer;
	UPROPERTY() FTimerHandle FootstepTimer;
	UPROPERTY() FTimerHandle WallDetectTimer;

	UPROPERTY() FToroFloatInterp InterpFieldOfView;
	UPROPERTY() FToroFloatInterp InterpCrouch;

	UPROPERTY() FVector CamPosition;
	UPROPERTY() FVector2D CamLeanOffset;
	UPROPERTY() FVector2D CamSwayOffset;
	UPROPERTY() FVector2D CamCurrentOffset;
	UPROPERTY() FVector2D CamTargetOffset;

	UPROPERTY() float StaminaDelta;
	UPROPERTY() float CurrentStamina;
	UPROPERTY() float MoveSpeedTarget;
	UPROPERTY() EPlayerLeanState LeanState;

	UPROPERTY(Transient) TObjectPtr<UObject> TaskDevice;
	UPROPERTY(Transient) TObjectPtr<UObject> HidingSpot;
	UPROPERTY(Transient) TObjectPtr<UObject> WorldDevice;
	UPROPERTY(Transient) TSet<TObjectPtr<class AGameEnemyBase>> EnemyStack;

public:

	UFUNCTION(BlueprintCallable, Category = Player)
		void ResetStates();
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void OverrideControlFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/GameRuntime.EPlayerControlFlags")) const int32 InFlags);

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
		bool IsMoving() const { return GetVelocity().Size2D() > 50.0f; }

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsLocked() const { return ControlFlags & PCF_Locked || !LockFlags.IsEmpty(); }

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetRunState(const bool bInState);

	UFUNCTION(BlueprintPure, Category = Player)
		bool HasRunFlag() const { return HasStateFlag(PSF_Running); }

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsRunning() const { return HasStateFlag(PSF_Running) && GetVelocity().Size() > WalkingSpeed; }

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
		float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }

	UFUNCTION(BlueprintPure, Category = Player)
		bool IsStaminaPunished() const { return HasStateFlag(PSF_RunLocked); }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLockOnTarget(const USceneComponent* InComponent);

	UFUNCTION(BlueprintPure, Category = Player)
		const USceneComponent* GetLockOnTarget() const { return LockOnTarget; }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetHidingSpot(UObject* InObject);

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetHidingSpot() const { return HidingSpot; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void ForceExitHiding();

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetWorldDevice(UObject* InObject);

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetWorldDevice() const { return WorldDevice; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void ForceExitWorldDevice();

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetTaskDevice(UObject* InObject);

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetTaskDevice() const { return TaskDevice; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void ForceExitTaskDevice();

	UFUNCTION(BlueprintCallable, Category = Player)
		void ClearEnemyStack();
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void AddEnemy(AGameEnemyBase* InEnemy);

	UFUNCTION(BlueprintCallable, Category = Player)
		void RemoveEnemy(AGameEnemyBase* InEnemy);
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void UpdateEnemy(const AGameEnemyBase* InEnemy);

	UFUNCTION(BlueprintCallable, Category = Player)
		bool TryJumpscare();

	virtual void EnterDialogue() override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual bool GetLookTarget_Implementation(FVector& Location) override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) override;
	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

protected:

	void TickStamina();
	void TickFootstep();
	void LeanWallDetect();
	bool IsStandingBlocked() const;
	bool IsLeaningBlocked(const float Direction) const;

	void OnEnemyStackChanged();
	void OnSettingsChange(const class UToroUserSettings* InSettings);

	virtual void SlowTick() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	INPUT_BINDING_FUNCTIONS()
};
