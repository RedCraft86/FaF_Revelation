// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PlayerTypes.h"
#include "Characters/EnemyBase.h"
#include "Camera/CameraComponent.h"
#include "DataTypes/ToroModifiers.h"
#include "DataTypes/ToroInterpolator.h"
#include "Framework/ToroPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/ExitInterface.h"
#include "GamePlayerChar.generated.h"

#define EXTERNAL_OBJECT_FUNC(Property) \
	void Set##Property(AActor* InActor); \
		AActor* Get##Property() { return Property; } \
		bool Exit##Property() const { \
		bool bExist = IsValid(Property); \
		IExitInterface::Exit(Property); \
		return bExist; \
	}

UCLASS(Abstract)
class FAFREVGAME_API AGamePlayerChar final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	AGamePlayerChar();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UAudioComponent> FootstepAudio;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInspectionComponent> Inspection;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UGameMusicManager> MusicManager;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UToroNarrative> Narrative;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInventoryComponent> Inventory;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UInteractionComponent> Interaction;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 ControlFlags;
	
	UPROPERTY(VisibleAnywhere, Category = Settings, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerStateFlags"))
		int32 StateFlags;

	UPROPERTY(EditAnywhere, Category = Settings)
		float InteractDistance;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

	UPROPERTY(EditAnywhere, Category = Settings)
		float FOVRunOffset;

	UPROPERTY(EditAnywhere, Category = Settings)
		float FOVCrouchOffset;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayThumbnail = false))
		TObjectPtr<const USceneComponent> LockOnTarget;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float LockOnSpeed;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D LeanOffsets;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> SideTrace;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D StrafeOffsets;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 100.0f, UIMin = 100.0f))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float RunSpeedMulti;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 1.0, UIMax = 1.0))
		float SneakSpeedMulti;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		FVector2D CrouchHeights;

	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> CeilingTrace;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerStamina Stamina;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPlayerFootsteps Footsteps;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ReadOnlyKeys, DisplayThumbnail = false))
		TMap<FName, TObjectPtr<UInputAction>> InputActions;

private:

	FToroSumFloat FOV;
	FToroSumVector2D CamOffset;
	FToroMultiFloat MoveSpeed;
	FToroMultiVector2D Sensitivity;
	EPlayerLeanState LeanState;

	FToroInterpFloat InterpFOV;
	FToroInterpFloat InterpFocal;
	FToroInterpFloat InterpCrouch;
	FToroInterpVector2D InterpCamOffset;

	FTimerHandle StaminaTimer;
	FTimerHandle FootstepTimer;
	FTimerHandle WallDetectTimer;

	TObjectPtr<AActor> HidingSpot;
	TObjectPtr<AActor> WorldDevice;
	TObjectPtr<AActor> ActiveTask;

	TSet<TObjectPtr<const AEnemyBase>> Enemies;

public:

	UFUNCTION(BlueprintPure, Category = Player)
		float GetSpeed() const { return GetVelocity().Size2D(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsMoving() const { return GetSpeed() > 50.0f; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsPaused() const { return GetWorldSettings()->GetPauserPlayerState() != nullptr; }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsLocked() const;
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void ResetStates();
	
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
		void SetRunState(const bool bInState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsRunning() const { return HasStateFlag(PSF_Run) && GetSpeed() > MoveSpeed.Base + 25.0f; }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetCrouchState(const bool bInState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsCrouching() const { return HasStateFlag(PSF_Crouch); }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLeanState(const EPlayerLeanState InState);
	
	UFUNCTION(BlueprintPure, Category = Player)
		EPlayerLeanState GetLeanState() const { return LeanState; }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		void SetStaminaPercent(const float InPercent = 1.0f) { Stamina.SetPercent(InPercent); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		float GetStaminaPercent() const { return Stamina.GetPercent(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		float GetStaminaDelta() const { return Stamina.GetDelta(); }
	
	UFUNCTION(BlueprintPure, Category = Player)
		bool IsStaminaPunished() const { return HasControlFlag(PCF_UseStamina) && HasStateFlag(PSF_RunLocked); }

	UFUNCTION(BlueprintCallable, Category = Player)
		void SetLockOnTarget(const USceneComponent* InComponent);

	UFUNCTION(BlueprintPure, Category = Player)
		const USceneComponent* GetLockOnTarget() const { return LockOnTarget; }
	
	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Set DOF Focal Distance")
		void SetFocalDistance(const float InFocalDistance = 100.0f, const float InSpeed = 5.0f);

	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Add FOV Offset")
		void AddFOVOffset(const FName InKey, const float InValue) { FOV.AddMod(InKey, InValue); }
	
	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Remove FOV Offset")
		void RemoveFOVOffset(const FName InKey) { FOV.RemoveMod(InKey); }
	
	UFUNCTION(BlueprintPure, Category = Player, DisplayName = "Has FOV Offset")
		bool HasFOVOffset(const FName InKey) const { return FOV.HasMod(InKey); }

	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Add Walk Multiplier")
		void AddWalkMulti(const FName InKey, const float InValue) { MoveSpeed.AddMod(InKey, InValue); }
	
	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Remove Walk Multiplier")
		void RemoveWalkMulti(const FName InKey) { MoveSpeed.RemoveMod(InKey); }
	
	UFUNCTION(BlueprintPure, Category = Player, DisplayName = "Has Walk Multiplier")
		bool HasWalkMulti(const FName InKey) const { return MoveSpeed.HasMod(InKey); }

	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Add Camera Offset")
		void AddCamOffset(const FName InKey, const FVector2D InValue) { CamOffset.AddMod(InKey, InValue); }
	
	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Remove Camera Offset")
		void RemoveCamOffset(const FName InKey) { CamOffset.RemoveMod(InKey); }
	
	UFUNCTION(BlueprintPure, Category = Player, DisplayName = "Has Camera Offset")
		bool HasCamOffset(const FName InKey) const { return CamOffset.HasMod(InKey); }

	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Add Sensitivity Multiplier")
		void AddSensitivityMulti(const FName InKey, const FVector2D InValue) { Sensitivity.AddMod(InKey, InValue); }
	
	UFUNCTION(BlueprintCallable, Category = Player, DisplayName = "Remove Sensitivity Multiplier")
		void RemoveSensitivityMulti(const FName InKey) { Sensitivity.RemoveMod(InKey); }
	
	UFUNCTION(BlueprintPure, Category = Player, DisplayName = "Has Sensitivity Multiplier")
		bool HasSensitivityMulti(const FName InKey) const { return Sensitivity.HasMod(InKey); }

	EXTERNAL_OBJECT_FUNC(HidingSpot)
	EXTERNAL_OBJECT_FUNC(WorldDevice)
	EXTERNAL_OBJECT_FUNC(ActiveTask)

	bool TryJumpscare();
	void UpdateEnemyState(const AEnemyBase* InEnemy);

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual void EnterCinematic(AActor* CinematicActor) override;
	virtual bool GetLookTarget_Implementation(FVector& Location) const override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;
	virtual void Teleport(const FVector& InLocation, const FRotator& InRotation) override;

private:

	void TickStamina();
	void TickFootstep();
	void LeanWallDetect();
	bool IsStandingBlocked() const;
	bool IsLeaningBlocked(const float Direction) const;

	FHitResult OnInteraction() const;
	void OnInspection(const bool bInspecting);
	void OnSettingsChange(const class UToroUserSettings* InSettings);
	USceneComponent* GetEquipmentRoot() const { return EquipmentRoot; }

	virtual void SlowTick() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

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
