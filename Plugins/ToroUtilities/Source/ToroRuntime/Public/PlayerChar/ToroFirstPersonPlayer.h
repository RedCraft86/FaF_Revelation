// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PlayerStatics.h"
#include "Characters/ToroPlayerBase.h"
#include "DataTypes/AdvancedFloatTypes.h"
#include "Interaction/InteractionComponent.h"
#include "ToroFirstPersonPlayer.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AToroFirstPersonPlayer : public AToroPlayerBase
{
	GENERATED_BODY()

public:
	
	AToroFirstPersonPlayer();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpringArmComponent> CameraArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionComponent> Interaction;

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
		FPlayerCameraShakes CameraShakes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPlayerFootsteps Footsteps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, TObjectPtr<UInputAction>> InputActions;

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
	UPROPERTY(Transient) TObjectPtr<UGameWidgetBase> GameWidget;

public:

	UFUNCTION(BlueprintCallable, Category = Player)
		void ResetStates();

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
		void ForceExitHiding() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetWorldDevice(UObject* InObject);

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetWorldDevice() const { return WorldDevice; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void ForceExitWorldDevice() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetTaskDevice(UObject* InObject);

	UFUNCTION(BlueprintPure, Category = Player)
		UObject* GetTaskDevice() const { return TaskDevice; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void ForceExitTaskDevice() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		bool TryJumpscare();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void FadeToBlack(const float InTime, const bool bAudio = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void FadeFromBlack(const float InTime, const bool bAudio = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Player)
		void ClearFade() const;

	virtual void EnterDialogue() override;
	virtual void SetControlFlag(const EPlayerControlFlags InFlag) override;
	virtual void UnsetControlFlag(const EPlayerControlFlags InFlag) override;
	virtual void TeleportPlayer(const FVector& InLocation, const FRotator& InRotation) override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual bool GetLookTarget_Implementation(FVector& Location) override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) override;

protected:

	void TickStamina();
	void TickFootstep();
	void LeanWallDetect();
	bool IsStandingBlocked() const;
	bool IsLeaningBlocked(const float Direction) const;
	UGameWidgetBase* GetGameWidget();
	
	void OnSettingsChange(const class UToroUserSettings* InSettings);
	virtual void OnEnemyStackChanged() override;
	virtual void SlowTick() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	INPUT_BINDING_FUNCTIONS()
};
