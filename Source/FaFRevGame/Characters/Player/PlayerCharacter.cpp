// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/InteractionManager.h"
#include "MusicSystem/WorldMusicManager.h"
#include "Inspection/InspectionManager.h"
#include "Inventory/InventoryManager.h"
#include "Narrative/NarrativeManager.h"
#include "Tutorials/TutorialManager.h"
#include "Libraries/ToroMathLibrary.h"
#include "Interfaces/ExitInterface.h"

#define CanInput() !IsControlLocked() && !IsPaused()

#define TRACE_PARAMS(StatName) \
	FCollisionQueryParams(UE_INLINE_STRINGIFY(PlayerTrace_##StatName), false, this)


#define BIND_INPUT_ACTION(Component, Event, Action) \
	if (const UInputAction* IA_##Action = InputActions.FindRef(Player::Inputs::Action)) \
	{\
		Component->BindAction(IA_##Action, ETriggerEvent::Event, this, &APlayerCharacter::InputBinding_##Action); \
	}

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerCamera->SetFieldOfView(88.f);

	ControlFlags = Player::DefaultAbilities;
	StateFlags = PSF_None;
	InteractDistance = 250.0f;
	InteractTrace = ECC_Visibility;
	LockOnSpeed = 5.0f;
	LeanOffsets = FVector2D(75.0f, 25.0f);
	SideTrace = ECC_Visibility;
	WalkSpeed = 300.0f;
	StrafeOffsets = FVector2D(2.5f, 1.5f);
	RunConfig = FPlayerMovementInfo(2.5f, 5.0f);
	Stamina = FPlayerStamina(100.0f, 25.0f);
	SneakConfig = FPlayerMovementInfo(0.5f, -5.0f);
	HalfHeights = FVector2D(88.0f, 45.0f);
	CeilingTrace = ECC_Visibility;

	LeanState = EPlayerLeanState::None;
	FieldOfView = FToroSumFloat(88.0f);
	InterpFOV = FToroInterpFloat(88.0f, 5.0f);
	CameraOffset = FToroSumVector2D(FVector2D::ZeroVector);
	InterpCamOffset = FToroInterpVector2D(FVector2D::ZeroVector, 7.5f);
	MovementMulti = FToroMultiFloat(1.0f);
	Sensitivity = FToroMultiVector2D(FVector2D::UnitVector);
	InterpCrouch = FToroInterpFloat(88.0f, 5.0f);
}

void APlayerCharacter::OverrideControlFlags(const int32 InFlags)
{
	for (const EPlayerControlFlags Enum : TEnumRange<EPlayerControlFlags>())
	{
		InFlags & Enum ? SetControlFlag(Enum) : UnsetControlFlag(Enum);
	}
}

void APlayerCharacter::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None && !HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
		switch (InFlag)
		{
			case PCF_UseStamina:	SetStaminaEnabled(true);		break;
			case PCF_CanInteract:	Interaction->SetEnabled(true);	break;
			default: break;
		}
	}
}

void APlayerCharacter::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None && HasControlFlag(InFlag))
	{
		ControlFlags &= ~InFlag;
		switch (InFlag)
		{
			case PCF_CanRun:		SetRunState(false);						break;
			case PCF_CanCrouch:		SetCrouchState(false);					break;
			case PCF_CanLean:		SetLeanState(EPlayerLeanState::None);	break;
			case PCF_UseStamina:	SetStaminaEnabled(false);				break;
			case PCF_CanInteract:
				Interaction->SetEnabled(false);
				if (OperatingActor.IsValid())
				{
					IExitInterface::Exit(OperatingActor.Get());
				}
				break;
			case PCF_CanHide:
				if (HidingSpot.IsValid())
				{
					IExitInterface::Exit(HidingSpot.Get());
				}
				break;
			default: break;
		}
	}
}

bool APlayerCharacter::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	return InFlag != PCF_None && ControlFlags & InFlag;
}

void APlayerCharacter::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None) StateFlags |= InFlag;
}

void APlayerCharacter::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None) StateFlags &= ~InFlag;
}

bool APlayerCharacter::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	return InFlag != PSF_None && StateFlags & InFlag;
}

int32 APlayerCharacter::GetActivityFlags() const
{
	int32 Flags = PAF_None;
	if (HidingSpot.IsValid())
	{
		Flags |= PAF_Hiding;
	}
	if (OperatingActor.IsValid())
	{
		Flags |= PAF_Operating;
	}
	if (InspectionManager && InspectionManager->IsInspecting())
	{
		Flags |= PAF_Inspect;
	}
	if (InventoryManager && InventoryManager->IsInventoryOpen())
	{
		Flags |= PAF_Inventory;
	}
	if (TutorialManager && TutorialManager->IsShowingTutorial())
	{
		Flags |= PAF_Tutorial;
	}
	if (NarrativeManager && NarrativeManager->IsInDialogue())
	{
		Flags |= PAF_Dialogue;
	}
	return Flags;
}

bool APlayerCharacter::HasActivityFlag(const EPlayerActivityFlags InFlag) const
{
	return InFlag != PAF_None && GetActivityFlags() & InFlag;
}

void APlayerCharacter::AddFieldOfViewMod(const FName Key, const float Addition)
{
	FieldOfView.AddMod(Key, Addition);
	InterpFOV.Target = FieldOfView.Evaluate();
}

void APlayerCharacter::RemoveFieldOfViewMod(const FName Key)
{
	FieldOfView.RemoveMod(Key);
	InterpFOV.Target = FieldOfView.Evaluate();
}

bool APlayerCharacter::HasFieldOfViewMod(const FName Key) const
{
	return FieldOfView.HasMod(Key);
}

void APlayerCharacter::AddCameraOffsetMod(const FName Key, const FVector2D Addition)
{
	CameraOffset.AddMod(Key, Addition);
	InterpCamOffset.Target = CameraOffset.Evaluate();
}

void APlayerCharacter::RemoveCameraOffsetMod(const FName Key)
{
	CameraOffset.RemoveMod(Key);
	InterpCamOffset.Target = CameraOffset.Evaluate();
}

bool APlayerCharacter::HasCameraOffsetMod(const FName Key) const
{
	return CameraOffset.HasMod(Key);
}

void APlayerCharacter::AddMovementSpeedMod(const FName Key, const float Multiplier)
{
	MovementMulti.AddMod(Key, Multiplier);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * MovementMulti.Evaluate();
}

void APlayerCharacter::RemoveMovementSpeedMod(const FName Key)
{
	MovementMulti.RemoveMod(Key);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * MovementMulti.Evaluate();
}

bool APlayerCharacter::HasMovementSpeedMod(const FName Key) const
{
	return MovementMulti.HasMod(Key);
}

void APlayerCharacter::AddSensitivityMod(const FName Key, const FVector2D Multiplier)
{
	Sensitivity.AddMod(Key, Multiplier);
}

void APlayerCharacter::RemoveSensitivityMod(const FName Key)
{
	Sensitivity.RemoveMod(Key);
}

bool APlayerCharacter::HasSensitivityMod(const FName Key) const
{
	return Sensitivity.HasMod(Key);
}

void APlayerCharacter::ResetStates()
{
	RefillStamina();
	SetRunState(false);
	SetCrouchState(false);
	SetLeanState(EPlayerLeanState::None);

	Interaction->SetInteracting(false);
	if (InventoryManager) InventoryManager->CloseInventory();
	if (InspectionManager) InspectionManager->EndInspection();
	if (HidingSpot.IsValid()) IExitInterface::Exit(HidingSpot.Get());
	if (OperatingActor.IsValid()) IExitInterface::Exit(OperatingActor.Get());
}

void APlayerCharacter::SetRunState(const bool bInState)
{
	const bool bTargetState = bInState && !Stamina.IsEmpty() && !IsCrouchState();
	if (IsRunState() != bTargetState)
	{
		if (bTargetState)
		{
			SetStateFlag(PSF_Run);
			AddMovementSpeedMod(Player::Internal::Running, RunConfig.SpeedMultiplier);
		}
		else
		{
			UnsetStateFlag(PSF_Run);
			RemoveMovementSpeedMod(Player::Internal::Running);
		}
	}
}

bool APlayerCharacter::IsRunState() const
{
	return HasStateFlag(PSF_Run);
}

bool APlayerCharacter::IsRunning() const
{
	return IsRunState() && FMath::IsNearlyEqual(GetSpeed(),
		WalkSpeed * RunConfig.SpeedMultiplier, 25.0f);
}

void APlayerCharacter::SetCrouchState(const bool bInState)
{
	const bool bTargetState = bInState && !IsRunState();
	if (IsCrouchState() != bTargetState)
	{
		if (bTargetState)
		{
			SetStateFlag(PSF_Crouch);
			AddFieldOfViewMod(Player::Internal::Crouching, SneakConfig.FieldOfViewOffset);
			AddMovementSpeedMod(Player::Internal::Crouching, SneakConfig.SpeedMultiplier);
			InterpCrouch.Target = HalfHeights.GetMin();
		}
		else
		{
			UnsetStateFlag(PSF_Crouch);
			RemoveFieldOfViewMod(Player::Internal::Crouching);
			RemoveMovementSpeedMod(Player::Internal::Crouching);
			InterpCrouch.Target = HalfHeights.GetMax();
		}
	}
}

bool APlayerCharacter::IsCrouchState() const
{
	return HasStateFlag(PSF_Crouch);
}

bool APlayerCharacter::IsSneaking() const
{
	return IsCrouchState() && FMath::IsNearlyEqual(GetSpeed(),
		WalkSpeed * SneakConfig.SpeedMultiplier, 25.0f);
}

void APlayerCharacter::SetLeanState(const EPlayerLeanState InState)
{
	if (LeanState == InState) return;
	if (InState == EPlayerLeanState::None)
	{
		LeanState = EPlayerLeanState::None;
		RemoveCameraOffsetMod(Player::Internal::Leaning);
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else if (const int32 Dir = Player::LeanToDir(InState); !IsLeaningBlocked(Dir))
	{
		LeanState = InState;
		AddCameraOffsetMod(Player::Internal::Leaning, LeanOffsets * Dir);
		GetWorldTimerManager().UnPauseTimer(WallDetectTimer);
	}
}

EPlayerLeanState APlayerCharacter::GetLeanState() const
{
	return LeanState;
}

void APlayerCharacter::SetStaminaEnabled(const bool bInEnabled)
{
	FTimerManager& Timer = GetWorldTimerManager();
	bInEnabled ? Timer.UnPauseTimer(StaminaTimer) : Timer.PauseTimer(StaminaTimer);
	if (!bInEnabled)
	{
		Stamina.SetPercent(1.0f);
	}
	OnStaminaUpdate.Broadcast(bInEnabled, Stamina);
}

bool APlayerCharacter::IsStaminaEnabled() const
{
	return GetWorldTimerManager().IsTimerActive(StaminaTimer);
}

void APlayerCharacter::RefillStamina()
{
	Stamina.SetPercent(1.0f);
	OnStaminaUpdate.Broadcast(IsStaminaEnabled(), Stamina);
}

void APlayerCharacter::SetHidingSpot(AActor* InSpot)
{
	HidingSpot = InSpot;
}

void APlayerCharacter::SetOperatingActor(AActor* InActor)
{
	OperatingActor = InActor;
}

bool APlayerCharacter::TryJumpscare(const FGameplayTag& FromEnemy)
{
	if (IsKillLocked() || IsPaused())
	{
		return false;
	}

	ResetStates();
	JumpscareEnemy = FromEnemy;
	if (MusicManager)
	{
		MusicManager->ClearDipRequests();
		MusicManager->SetPaused(true);
	}
	return true;
}

bool APlayerCharacter::IsKillLocked()
{
	return Super::IsKillLocked()
		|| JumpscareEnemy.IsValid()
		|| HasActivityFlag(PAF_Dialogue)
		|| IsPaused();
}

bool APlayerCharacter::IsControlLocked()
{
	return Super::IsControlLocked()
		|| JumpscareEnemy.IsValid()
		|| GetActivityFlags() != PAF_None;
}

void APlayerCharacter::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	EquipmentRoot->SetHiddenInGame(bNewHidden, true);
}

void APlayerCharacter::TickStamina()
{
	Stamina.TimedTick(StateFlags);
	OnStaminaUpdate.Broadcast(true, Stamina);
	if (IsRunState() && Stamina.IsEmpty())
	{
		SetRunState(false);
	}
}

void APlayerCharacter::TickFootstep()
{
	TEnumAsByte<EPhysicalSurface> Surface;
	if (IsMoving() && GetStandingSurface(Surface, Footsteps.TraceChannel))
	{
		PlayFootstep(Footsteps.GetFootstepSound(StateFlags, Surface));
	}
	FootstepTimer.Invalidate();
}

void APlayerCharacter::TickWallDetect()
{
	if (LeanState == EPlayerLeanState::None)
	{
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else if (IsLeaningBlocked(Player::LeanToDir(LeanState)))
	{
		SetLeanState(EPlayerLeanState::None);
	}
}

bool APlayerCharacter::IsStandingBlocked() const
{
	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Up,
		GetCapsuleComponent()->GetScaledCapsuleRadius() + HalfHeights.X + 5.0f,
		Start, End);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		CeilingTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS(Ceiling));
}

bool APlayerCharacter::IsLeaningBlocked(const int32 Dir) const
{
	if (Dir == 0) return false;
	if (Dir < -1 || Dir > 1) return true;

	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Right,
		(LeanOffsets.X + 10.0f) * Dir, Start, End);

	Start.Z = End.Z = PlayerCamera->GetComponentLocation().Z;

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		SideTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS(Side));
}

FHitResult APlayerCharacter::HandleInteraction() const
{
	FHitResult Hit = FHitResult();
	const AToroPlayerController* PC = GetPlayerController();
	if (!PC || PC->bCinematicMode) return Hit;
	if (PC->GetViewTarget() == this)
	{
		FVector Start, End;
		UToroMathLibrary::GetComponentLineTraceVectors(PlayerCamera,
			EVectorDirection::Forward, InteractDistance, Start, End);

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, InteractTrace, TRACE_PARAMS(Interaction));
	}
	else if (PC->ShouldShowMouseCursor())
	{
		PC->GetHitResultUnderCursor(InteractTrace, false, Hit);
	}
	return Hit;
}

void APlayerCharacter::OnCinematic(AActor* InActor)
{
	Super::OnCinematic(InActor);
	ResetStates();
}

void APlayerCharacter::OnSettingsUpdate(const ESettingApplyType ApplyType)
{
	Super::OnSettingsUpdate(ApplyType);
	if (const UToroUserSettings* UserSettings = UToroUserSettings::Get())
	{
		Sensitivity.Base = UserSettings->GetSensitivity();
	}
}

void APlayerCharacter::SlowTick()
{
	Super::SlowTick();
	if (IsRunning())
	{
		if (!HasFieldOfViewMod(Player::Internal::Running))
		{
			AddFieldOfViewMod(Player::Internal::Running, RunConfig.FieldOfViewOffset);
		}
	}
	else if (HasFieldOfViewMod(Player::Internal::Running))
	{
		RemoveFieldOfViewMod(Player::Internal::Running);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	Interaction->HandleTrace.BindUObject(this, &APlayerCharacter::HandleInteraction);
	Interaction->SetEnabled(HasControlFlag(PCF_CanInteract));

	if (const AToroCameraManager* CamManager = AToroCameraManager::Get(this))
	{
		EquipmentRoot->AttachToComponent(CamManager->GetTransformComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	}

	InterpCrouch.Target = HalfHeights.GetMax(); InterpCrouch.SnapToTarget();
	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpCrouch.Current);

	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(WallDetectTimer, this, &APlayerCharacter::TickWallDetect, 0.1f, true);
	Timer.PauseTimer(WallDetectTimer);
	
	Timer.SetTimer(StaminaTimer, this, &APlayerCharacter::TickStamina, Stamina.TickInterval, true);
	if (!HasControlFlag(PCF_UseStamina)) SetStaminaEnabled(false);
	
	Timer.SetTimerForNextTick([this]()
	{
		MusicManager = UWorldMusicManager::Get(this);
		TutorialManager = UTutorialManager::Get(this);
		InventoryManager = UInventoryManager::Get(this);
		InspectionManager = UInspectionManager::Get(this);
		NarrativeManager = UNarrativeManager::Get(this);
	});
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPaused()) return;

	if (!InterpFOV.IsComplete())
	{
		PlayerCamera->SetFieldOfView(InterpFOV.Tick(DeltaTime));
	}
	if (!InterpCrouch.IsComplete())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(InterpCrouch.Tick(DeltaTime));
	}

	InterpCamOffset.Tick(DeltaTime);
	if (!LockOnTarget.IsValid())
	{
		FRotator CtrlRot = GetControlRotation();
		CtrlRot.Roll = InterpCamOffset.Current.Y;
		SetControlRotation(CtrlRot);

		APlayerController* PC = GetPlayerController();
		if (IsMoving() && PC && !PC->bCinematicMode)
		{
			PC->ClientStartCameraShake(IsRunning()
				? UCamShake_Run::StaticClass()
				: UCamShake_Walk::StaticClass(),
				1.0f);
		}
	}
	else if (FVector Target; GetViewTarget(this, Target) && LockOnSpeed > 0.1f)
	{
		SetControlRotation(FMath::RInterpTo(GetControlRotation(),
			FRotationMatrix::MakeFromX(Target - PlayerCamera->GetComponentLocation()).Rotator(),
			DeltaTime, LockOnSpeed));
	}

	CameraArm->SetRelativeLocation({
		0.0f, InterpCamOffset.Current.X, GetCapsuleCameraOffset()
	});

	if (!FootstepTimer.IsValid() && IsMoving())
	{
		GetWorldTimerManager().SetTimer(FootstepTimer,
			this, &APlayerCharacter::TickFootstep,
			Footsteps.GetFootstepInterval(StateFlags), false);
	}
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		Footsteps.FillSlots();
		for (const FName& ActionName : Player::Inputs::All)
		{
			if (!InputActions.Contains(ActionName)) InputActions.Add(ActionName);
		}
	}
#endif
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Pause)

		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Back)

		BIND_INPUT_ACTION(EnhancedInputComponent, Triggered, Turn)

		BIND_INPUT_ACTION(EnhancedInputComponent, Triggered, Move)
		BIND_INPUT_ACTION(EnhancedInputComponent, Completed, Move)
		BIND_INPUT_ACTION(EnhancedInputComponent, Canceled, Move)

		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Run)
		BIND_INPUT_ACTION(EnhancedInputComponent, Completed, Run)
		BIND_INPUT_ACTION(EnhancedInputComponent, Canceled, Run)

		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Crouch)

		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Lean)
		BIND_INPUT_ACTION(EnhancedInputComponent, Completed, Lean)
		BIND_INPUT_ACTION(EnhancedInputComponent, Canceled, Lean)

		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Inventory)

		BIND_INPUT_ACTION(EnhancedInputComponent, Started, HideQuests)

		BIND_INPUT_ACTION(EnhancedInputComponent, Triggered, Interact)
		BIND_INPUT_ACTION(EnhancedInputComponent, Completed, Interact)
		BIND_INPUT_ACTION(EnhancedInputComponent, Canceled, Interact)

		BIND_INPUT_ACTION(EnhancedInputComponent, Triggered, Equipment)
		BIND_INPUT_ACTION(EnhancedInputComponent, Completed, Equipment)
		BIND_INPUT_ACTION(EnhancedInputComponent, Canceled, Equipment)
	}
}

void APlayerCharacter::InputBinding_Pause(const FInputActionValue& InValue)
{
	if (InventoryManager && InventoryManager->IsInventoryOpen())
	{
		InventoryManager->CloseInventory();
	}
	if (InspectionManager && InspectionManager->IsInspecting())
	{
		InspectionManager->EndInspection();
	}

	AToroPlayerController* PC = GetPlayerController();
	if (PC && CanInput())
	{
		SetRunState(false);
		SetLeanState(EPlayerLeanState::None);
		Interaction->SetInteracting(false);
		PC->SetUserPause(true);
	}
}

void APlayerCharacter::InputBinding_Back(const FInputActionValue& InValue)
{
	if (HidingSpot.IsValid())
	{
		IExitInterface::Exit(HidingSpot.Get());
	}
	else if (OperatingActor.IsValid())
	{
		IExitInterface::Exit(OperatingActor.Get());
	}
	else if (InspectionManager && InspectionManager->IsInspecting())
	{
		InspectionManager->EndInspection();
	}
}

void APlayerCharacter::InputBinding_Turn(const FInputActionValue& InValue)
{
	if (CanInput() && HasControlFlag(PCF_CanTurn) && !LockOnTarget.IsValid())
	{
		const FVector2D Axis = InValue.Get<FVector2D>() * Sensitivity.Evaluate();
		AddControllerPitchInput(Axis.Y);
		AddControllerYawInput(Axis.X);
	}
}

void APlayerCharacter::InputBinding_Move(const FInputActionValue& InValue)
{
	if (CanInput() && HasControlFlag(PCF_CanMove))
	{
		const FVector2D Axis = InValue.Get<FVector2D>();
		AddMovementInput(GetActorRightVector(), Axis.Y);
		AddMovementInput(GetActorForwardVector(), Axis.X);
		AddCameraOffsetMod(Player::Internal::Movement, StrafeOffsets * Axis.Y);
	}
	else if (HasCameraOffsetMod(Player::Internal::Movement))
	{
		RemoveCameraOffsetMod(Player::Internal::Movement);
	}
}

void APlayerCharacter::InputBinding_Run(const FInputActionValue& InValue)
{
	SetRunState(CanInput() && HasControlFlag(PCF_CanRun) && InValue.Get<bool>());
}

void APlayerCharacter::InputBinding_Crouch(const FInputActionValue& InValue)
{
	if (CanInput())
	{
		if (HasControlFlag(PCF_CanCrouch) && !IsCrouchState())
		{
			SetCrouchState(true);
		}
		else if (!IsStandingBlocked())
		{
			SetCrouchState(false);
		}
	}
}

void APlayerCharacter::InputBinding_Lean(const FInputActionValue& InValue)
{
	if (CanInput() && !LockOnTarget.IsValid())
	{
		const float Direction = InValue.Get<float>();
		if (!HasControlFlag(PCF_CanLean) || FMath::IsNearlyZero(Direction))
		{
			SetLeanState(EPlayerLeanState::None);
		}
		else if (Direction > 0.0f)
		{
			SetLeanState(EPlayerLeanState::Right);
		}
		else // if (Direction < 0.0f)
		{
			SetLeanState(EPlayerLeanState::Left);
		}
	}
}

void APlayerCharacter::InputBinding_Inventory(const FInputActionValue& InValue)
{
	if (InventoryManager)
	{
		if (InventoryManager->IsInventoryOpen())
		{
			InventoryManager->CloseInventory();
		}
		else if (CanInput())
		{
			InventoryManager->OpenInventory();
		}
	}
}

void APlayerCharacter::InputBinding_HideQuests(const FInputActionValue& InValue)
{
	if (CanInput() && NarrativeManager)
	{
		NarrativeManager->ToggleQuests();
	}
}

void APlayerCharacter::InputBinding_Interact(const FInputActionValue& InValue)
{
	Interaction->SetInteracting(CanInput() && HasControlFlag(PCF_CanInteract) && InValue.Get<bool>());
}

void APlayerCharacter::InputBinding_Equipment(const FInputActionValue& InValue)
{
	if (InventoryManager)
	{
		InventoryManager->SetEquipmentUsage(CanInput() && InValue.Get<bool>());
	}
}

#undef CanInput
#undef TRACE_PARAMS
#undef BIND_INPUT_ACTION