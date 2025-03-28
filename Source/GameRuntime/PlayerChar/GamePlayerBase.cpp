﻿// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst
#include "GamePlayerBase.h"
#include "ToroInterface.h"
#include "ToroMathLibrary.h"
#include "Framework/ToroGameMode.h"
#include "Characters/GameEnemyBase.h"
#include "Components/AudioComponent.h"
#include "Framework/ToroMusicManager.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UserWidgets/NarrativeWidgetBase.h"
#include "UserSettings/ToroUserSettings.h"

#define CAN_INPUT !IsLocked() && !IsPaused()
#define TRACE_PARAMS FCollisionQueryParams(NAME_None, false, this)

AGamePlayerBase::AGamePlayerBase() : ControlFlags(DEFAULT_PLAYER_CONTROL_FLAGS), ReachDistance(250.0f)
	, InteractTrace(ECC_Visibility), FieldOfView(90.0f), FieldOfViewSpeed(5.0f), LockOnSpeed(5.0f)
	, Sensitivity(1.0f), SensitivityMulti(1.0f), LeanOffsets(75.0f, 25.0f), LeanSpeed(7.5f)
	, SideTrace(ECC_Visibility), SideTraceLength(125.0f), MoveSpeedMulti(1.0f), WalkingSpeed(300.0f)
	, SwayOffsets(2.5f, 1.5f), CrouchWalkSpeed(200.0f), CrouchFOV(-5.0f), CeilingTrace(ECC_Visibility)
	, CrouchHeights(88.0f, 45.0f), CrouchSpeed(5.0f), RunningSpeed(750.0f), RunningFOV(5.0f), MaxStamina(100.0f)
	, StaminaDrainRate(1.0f), StaminaGainRate(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	FootstepAudio->SetupAttachment(GetCapsuleComponent());

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bDoCollisionTest = false;
	CameraArm->TargetArmLength = 0.0f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Narrative = CreateDefaultSubobject<UToroNarrativeComponent>("Narrative");
	Interaction = CreateDefaultSubobject<UInteractionComponent>("Interaction");

	LockFlags = {Tag_PlayerLock_Startup.GetTag()};
}

void AGamePlayerBase::ResetStates()
{
	ClearEnemyStack();
	SetWorldDevice(nullptr);
	SetLockOnTarget(nullptr);

	SetRunState(false);
	SetCrouchState(false);
	SetLeanState(EPlayerLeanState::None);

	ForceExitHiding();
	ForceExitTaskDevice();
	ForceExitWorldDevice();
	GetInventory()->CloseInventory();

	const TSet<FName> AllFlags = Player::LockFlags::GetAll();
	const TSet<FName> Resettable = Player::LockFlags::Resettable();
	for (auto It = LockFlags.CreateIterator(); It; ++It)
	{
		if (!It->IsValidFlag() || !AllFlags.Contains(**It) || Resettable.Contains(**It))
		{
			It.RemoveCurrent();
		}
	}

	ControlFlags = DEFAULT_PLAYER_CONTROL_FLAGS;
	MoveSpeedMulti.Modifiers.Empty();
	FieldOfView.Modifiers.Empty();
	InterpFieldOfView.Target = FieldOfView.Evaluate();
	InterpFieldOfView.SnapToTarget();
	InterpCrouch.SnapToTarget();
	CamSwayOffset = FVector2D::ZeroVector;
	StaminaDrainRate.Modifiers.Empty();
	StaminaGainRate.Modifiers.Empty();
	CurrentStamina = MaxStamina;
}

void AGamePlayerBase::OverrideControlFlags(const int32 InFlags)
{
	for (const EPlayerControlFlags Enum : TEnumRange<EPlayerControlFlags>())
	{
		if (InFlags & Enum)
		{
			SetControlFlag(Enum);
		}
		else
		{
			UnsetControlFlag(Enum);
		}
	}
}

void AGamePlayerBase::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (!HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
		switch (InFlag)
		{
		case PCF_UseStamina:	GetWorldTimerManager().UnPauseTimer(StaminaTimer); break;
		default: return;
		}
	}
}

void AGamePlayerBase::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (HasControlFlag(InFlag))
	{
		ControlFlags &= ~InFlag;
		switch (InFlag)
		{
		case PCF_UseStamina:	GetWorldTimerManager().PauseTimer(StaminaTimer); break;
		case PCF_CanRun:		SetRunState(false); break;
		case PCF_CanCrouch:		SetCrouchState(false); break;
		case PCF_CanLean:		SetLeanState(EPlayerLeanState::None); break;
		default: return;
		}
	}
}

bool AGamePlayerBase::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	if (InFlag == PCF_None) return false;
	return ControlFlags & InFlag;
}

void AGamePlayerBase::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags |= InFlag;
}

void AGamePlayerBase::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags &= ~InFlag;
}

bool AGamePlayerBase::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	if (InFlag == PSF_None) return false;
	return StateFlags & InFlag;
}

void AGamePlayerBase::SetRunState(const bool bInState)
{
	if (bool bRunning = HasRunFlag(); bRunning != bInState)
	{
		const bool bCrouching = IsCrouching();
		bRunning = !IsStaminaPunished() && !bCrouching && bInState;
		if (bRunning)
		{
			SetStateFlag(PSF_Running);
		}
		else
		{
			UnsetStateFlag(PSF_Running);
		}

		if (!bCrouching)
		{
			MoveSpeedTarget = bRunning ? RunningSpeed : WalkingSpeed;
		}
	}
}

void AGamePlayerBase::SetCrouchState(const bool bInState)
{
	if (bool bCrouching = IsCrouching(); bCrouching != bInState)
	{
		const bool bRunning = IsRunning();
		bCrouching = !bRunning && bInState;
		if (bCrouching)
		{
			SetStateFlag(PSF_Crouching);
			FieldOfView.AddMod(Player::InternalKeys::CrouchFOV, CrouchFOV);
		}
		else
		{
			UnsetStateFlag(PSF_Crouching);
			FieldOfView.RemoveMod(Player::InternalKeys::CrouchFOV);
		}

		InterpCrouch.Target = bCrouching ? CrouchHeights.Y : CrouchHeights.X;
		if (!bRunning)
		{
			MoveSpeedTarget = bCrouching ? CrouchWalkSpeed : WalkingSpeed;
		}
	}
}

void AGamePlayerBase::SetLeanState(const EPlayerLeanState InState)
{
	if (LeanState == InState) return;
	if (InState == EPlayerLeanState::None)
	{
		LeanState = EPlayerLeanState::None;
		CamLeanOffset = FVector2D::ZeroVector;
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else
	{
		const float Dir = Player::LeanToFloat(InState);
		if (!IsLeaningBlocked(Dir))
		{
			LeanState = InState;
			CamLeanOffset = LeanOffsets * Dir;
			GetWorldTimerManager().UnPauseTimer(WallDetectTimer);
		}
	}
}

void AGamePlayerBase::SetStaminaPercent(const float InStamina)
{
	CurrentStamina = FMath::Lerp(0, MaxStamina, FMath::Clamp(InStamina, 0.0f, 1.0f));
}

void AGamePlayerBase::SetLockOnTarget(const USceneComponent* InComponent)
{
	if (InComponent) LockOnTarget = InComponent;
	else LockOnTarget = nullptr;
}

void AGamePlayerBase::SetHidingSpot(UObject* InObject)
{
	HidingSpot = InObject;
	if (WorldDevice) LockFlags.Add(Tag_PlayerLock_Hiding.GetTag());
	else LockFlags.Remove(Tag_PlayerLock_Hiding.GetTag());
	if (const AToroMusicManager* Manager = AToroMusicManager::Get(this))
	{
		Manager->SetHidingState(IsValid(HidingSpot));
	}
}

void AGamePlayerBase::ForceExitHiding()
{
	IToroInterface::Return(HidingSpot);
	SetHidingSpot(nullptr);
}

void AGamePlayerBase::SetWorldDevice(UObject* InObject)
{
	WorldDevice = InObject;
	if (WorldDevice) LockFlags.Add(Tag_PlayerLock_Device.GetTag());
	else LockFlags.Remove(Tag_PlayerLock_Device.GetTag());
}

void AGamePlayerBase::ForceExitWorldDevice()
{
	IToroInterface::Return(WorldDevice);
	SetWorldDevice(nullptr);
}

void AGamePlayerBase::SetTaskDevice(UObject* InObject)
{
	TaskDevice = InObject;
	if (TaskDevice) SetStateFlag(PSF_Tasking);
	else UnsetStateFlag(PSF_Tasking);
}

void AGamePlayerBase::ForceExitTaskDevice()
{
	IToroInterface::Return(TaskDevice);
	SetTaskDevice(nullptr);
}

void AGamePlayerBase::ClearEnemyStack()
{
	if (!EnemyStack.IsEmpty())
	{
		EnemyStack.Empty();
		OnEnemyStackChanged();
	}
}

void AGamePlayerBase::AddEnemy(AGameEnemyBase* InEnemy)
{
	if (!EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Add(InEnemy);
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AGamePlayerBase::RemoveEnemy(AGameEnemyBase* InEnemy)
{
	if (EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Remove(InEnemy);
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AGamePlayerBase::UpdateEnemy(const AGameEnemyBase* InEnemy)
{
	if (EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

bool AGamePlayerBase::TryJumpscare()
{
	if (ControlFlags & PCF_Locked || GameInstance->IsPlayerInvisible()) return false;
	const TSet<FName> ImmunityFlags = Player::LockFlags::Immunity();
	for (const FPlayerLockFlag& Flag : LockFlags)
	{
		if (ImmunityFlags.Contains(*Flag)) return false;
	}

	AddLockFlag(Tag_PlayerLock_Jumpscare.GetTag());

	ForceExitWorldDevice();
	if (LockFlags.Contains(GAMEPLAY_TAG_CHILD(Inventory, PlayerLock)))
	{
		GetInventory()->CloseInventory();
	}

	return true;
}

void AGamePlayerBase::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	EquipmentRoot->SetHiddenInGame(bNewHidden, true);
}

bool AGamePlayerBase::GetLookTarget_Implementation(FVector& Location)
{
	Location = LockOnTarget ? LockOnTarget->GetComponentLocation() : FVector::ZeroVector;
	return IsValid(LockOnTarget);
}

void AGamePlayerBase::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle)
{
	Location = PlayerCamera->GetComponentLocation();
	Forward = PlayerCamera->GetForwardVector();
	Angle = PlayerCamera->FieldOfView;
}

void AGamePlayerBase::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	if (AToroCameraManager* CamManager = AToroCameraManager::Get(this))
	{
		CamManager->SetGameCameraCutThisFrame();
	}

	SetActorLocation(InLocation, false, nullptr, ETeleportType::ResetPhysics);

	const bool bSmooth = CameraArm->bEnableCameraRotationLag;
	CameraArm->bEnableCameraRotationLag = false;
	FRotator Rot(InRotation); Rot.Roll = 0.0f;
	Controller->SetControlRotation(Rot);
	CameraArm->bEnableCameraRotationLag = bSmooth;
}

void AGamePlayerBase::TickStamina()
{
	StaminaDelta = IsRunning() ? -StaminaDrainRate.Evaluate() : StaminaGainRate.Evaluate();
	CurrentStamina = FMath::Clamp(StaminaDelta + CurrentStamina, 0.0f, MaxStamina);
	if (CurrentStamina < 0.01f && !IsStaminaPunished())
	{
		SetRunState(false);
		SetStateFlag(PSF_RunLocked);
	}
	else if (FMath::IsNearlyEqual(CurrentStamina, MaxStamina, 1.0f) && IsStaminaPunished())
	{
		UnsetStateFlag(PSF_RunLocked);
	}
}

void AGamePlayerBase::TickFootstep()
{
	FVector Start, End;
	UToroMathLibrary::GetComponentLineTraceVectors(FootstepAudio, EVectorDirection::Up, -150.0f , Start, End);
	FCollisionQueryParams Params{NAME_None, true, this};
	Params.bReturnPhysicalMaterial = true;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, Footsteps.FloorTrace, Params);
	if (IsMoving() && HitResult.bBlockingHit)
	{
		if (USoundBase* Sound = Footsteps.GetAudio(UPhysicalMaterial::DetermineSurfaceType(
			HitResult.PhysMaterial.Get()), IsRunning(), IsCrouching()))
		{
			FootstepAudio->SetVolumeMultiplier(Footsteps.Volume);
			FootstepAudio->SetSound(Sound);
			FootstepAudio->Play();
		}
	}

	FootstepTimer.Invalidate();
}

void AGamePlayerBase::LeanWallDetect()
{
	if (LeanState == EPlayerLeanState::None)
	{
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else if (IsLeaningBlocked(Player::LeanToFloat(LeanState)))
	{
		SetLeanState(EPlayerLeanState::None);
	}
}

bool AGamePlayerBase::IsStandingBlocked() const
{
	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Up,
		GetCapsuleComponent()->GetUnscaledCapsuleRadius() + CrouchHeights.X + 5.0f, Start, End);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		CeilingTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS);
}

bool AGamePlayerBase::IsLeaningBlocked(const float Direction) const
{
	if (FMath::IsNearlyZero(Direction)) return false;

	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Right,
		SideTraceLength * Direction, Start, End);

	Start += FVector(0.0f, 0.0f, CameraArm->GetRelativeLocation().Z);
	End += FVector(0.0f, 0.0f, CameraArm->GetRelativeLocation().Z);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		SideTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS);
}

void AGamePlayerBase::OnEnemyStackChanged()
{
	uint8 HighestState = 0;
	for (const AGameEnemyBase* Enemy : EnemyStack)
	{
		if (const uint8 State = (uint8)Enemy->GetEnemyState() - 1; State > HighestState)
		{
			HighestState = State;
		}
	}
	if (const AToroMusicManager* Manager = AToroMusicManager::Get(this))
	{
		Manager->SetThemeState(HighestState);
	}
}

void AGamePlayerBase::OnSettingsChange(const UToroUserSettings* InSettings)
{
	if (InSettings)
	{
		FieldOfView.AddMod(TEXT("Settings"), InSettings->GetFieldOfViewOffset());
		PlayerCamera->SetFieldOfView(FMath::Clamp(FieldOfView.Evaluate(), 5.0f, 170.0f));
		InterpFieldOfView.Target = PlayerCamera->FieldOfView;
		InterpFieldOfView.SnapToTarget();

		CameraArm->bEnableCameraRotationLag = InSettings->GetSmoothCamera();
		Sensitivity.X = InSettings->GetSensitivityX();
		Sensitivity.Y = InSettings->GetSensitivityY();
	}
}

void AGamePlayerBase::OnEnterDialogue(UDialogue* Dialogue)
{
	SetRunState(false);
	SetCrouchState(false);
	SetLeanState(EPlayerLeanState::None);
}

void AGamePlayerBase::SlowTick()
{
	if (IsRunning() && IsMoving())
	{
		if (!FieldOfView.Modifiers.Contains(Player::InternalKeys::RunFOV))
		{
			FieldOfView.AddMod(Player::InternalKeys::RunFOV, RunningFOV);
		}
	}
	else if (FieldOfView.Modifiers.Contains(Player::InternalKeys::RunFOV))
	{
		FieldOfView.RemoveMod(Player::InternalKeys::RunFOV);
	}

	InterpFieldOfView.Target = FieldOfView.Evaluate();
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeedTarget * MoveSpeedMulti.Evaluate();
}

void AGamePlayerBase::BeginPlay()
{
	Super::BeginPlay();

	GetNarrative()->OnDialogueBegan.AddUniqueDynamic(this, &ThisClass::OnEnterDialogue);

	CamPosition = CameraArm->GetRelativeLocation();
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	if (const APlayerCameraManager* CM = PlayerController->PlayerCameraManager)
	{
		EquipmentRoot->AttachToComponent(CM->GetTransformComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnDynamicSettingsChanged.AddUObject(this, &ThisClass::OnSettingsChange);
		OnSettingsChange(Settings);
	}

	FTimerManager& Timer = GetWorldTimerManager();
	{
		Timer.SetTimer(WallDetectTimer, this, &ThisClass::LeanWallDetect, 0.1f, true);
		Timer.PauseTimer(WallDetectTimer);
	
		Timer.SetTimer(StaminaTimer, this, &ThisClass::TickStamina, 0.1f, true);
		if (!HasControlFlag(PCF_UseStamina)) Timer.PauseTimer(StaminaTimer);
	}

	LockFlags.Remove(LockFlag(Startup));
}

void AGamePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnStaminaUpdate.IsBound())
	{
		OnStaminaUpdate.Execute(DeltaTime, HasControlFlag(PCF_UseStamina)
		   ? GetStaminaPercent() : -1.0f, StaminaDelta, IsStaminaPunished());
	}

	if (!InterpFieldOfView.IsComplete())
	{
		PlayerCamera->SetFieldOfView(InterpFieldOfView.Current);
		InterpFieldOfView.InterpSpeed = FieldOfViewSpeed;
		InterpFieldOfView.Tick(DeltaTime);
	}
	
	if (!InterpCrouch.IsComplete())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(InterpCrouch.Current);
		InterpCrouch.InterpSpeed = CrouchSpeed;
		InterpCrouch.Tick(DeltaTime);
	}

	if (LockOnTarget)
	{
		SetLeanState(EPlayerLeanState::None);
		const FVector TargetVector = LockOnTarget->GetComponentLocation() - PlayerCamera->GetComponentLocation();
		Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(),
			FRotationMatrix::MakeFromX(TargetVector).Rotator(), DeltaTime, LockOnSpeed));
	}
	else
	{
		CamTargetOffset = CamSwayOffset + CamLeanOffset;
		if (!CamCurrentOffset.Equals(CamTargetOffset, 0.01f))
		{
			CamCurrentOffset = FMath::Vector2DInterpTo(CamCurrentOffset, CamTargetOffset, DeltaTime, LeanSpeed);
			CamPosition.Y = CamCurrentOffset.X;

			const FRotator CtrlRot = GetController()->GetControlRotation();
			Controller->SetControlRotation(FRotator(CtrlRot.Pitch, CtrlRot.Yaw, CamCurrentOffset.Y));
		}

		if (IsMoving() && IsPlayerControlled())
		{
			const bool bRunning = IsRunning();
			PlayerController->ClientStartCameraShake(bRunning ? UCamShake_Run::StaticClass()
				: UCamShake_Walk::StaticClass(),1.0f);
		}
	}

	const float HalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();
	FootstepAudio->SetRelativeLocation({0.0f, 0.0f, -HalfHeight});

	const float CamHeight = HalfHeight + GetCapsuleComponent()->GetUnscaledCapsuleRadius() * 0.5f;
	CamPosition.Z = FMath::FInterpTo(CamPosition.Z, CamHeight, DeltaTime, 10.0f);

	CameraArm->SetRelativeLocation(CamPosition);

	if (!FootstepTimer.IsValid() && IsMoving())
	{
		GetWorldTimerManager().SetTimer(FootstepTimer, this, &ThisClass::TickFootstep,
			Footsteps.GetInterval(IsRunning(), IsCrouching()), false);
	}
}

void AGamePlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		INPUT_EVENT_BINDS(EIC)
	}
}

void AGamePlayerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		SensitivityMulti.ClampModifiers(0.0f, 5.0f);
		FieldOfView.ClampModifiers(-25.0f, 25.0f);
		MoveSpeedMulti.ClampModifiers(0.1f, 5.0f);

		StaminaDrainRate.ClampBase(0.1f, 5.0f);
		StaminaDrainRate.ClampModifiers(0.1f, 5.0f);

		StaminaGainRate.ClampBase(0.1f, 5.0f);
		StaminaGainRate.ClampModifiers(0.1f, 5.0f);

		for (const FName& ActionName : Player::InputActions::All)
		{
			if (!InputActions.Contains(ActionName)) InputActions.Add(ActionName);
		}
	}
#endif
}

void AGamePlayerBase::InputBinding_Pause(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		if (LockFlags.Contains(LockFlag(Guide)) || IsValid(PlayerController->GetCinematicActor())) return;
		if (LockFlags.Contains(LockFlag(Inventory)))
		{
			GetInventory()->CloseInventory();
			return;
		}

		if (IsPaused()) return;
		if (LockFlags.Contains(LockFlag(Device)))
		{
			ForceExitWorldDevice();
			return;
		}

		if (!IsLocked())
		{
			SetRunState(false);
			SetLeanState(EPlayerLeanState::None);
			PlayerController->SetGamePaused(true);
		}
	}
}

void AGamePlayerBase::InputBinding_Turn(const FInputActionValue& InValue)
{
	if (CAN_INPUT && HasControlFlag(PCF_CanTurn) && !IsValid(LockOnTarget))
	{
		const FVector2D Axis = InValue.Get<FVector2D>();
		const float Multiplier = SensitivityMulti.Evaluate();
		if (!FMath::IsNearlyZero(Axis.X))
		{
			AddControllerYawInput(Axis.X * Sensitivity.X * Multiplier);
		}
		if (!FMath::IsNearlyZero(Axis.Y))
		{
			AddControllerPitchInput(Axis.Y * Sensitivity.Y * Multiplier * -1.0f);
		}
	}
}

void AGamePlayerBase::InputBinding_Move(const FInputActionValue& InValue)
{
	if (CAN_INPUT && HasControlFlag(PCF_CanMove))
	{
		const FVector2D Axis = InValue.Get<FVector2D>();
		if (!FMath::IsNearlyZero(Axis.X))
		{
			AddMovementInput(GetActorForwardVector(), Axis.X);
		}
		if (!FMath::IsNearlyZero(Axis.Y))
		{
			AddMovementInput(GetActorRightVector(), Axis.Y);
		}

		CamSwayOffset = SwayOffsets * Axis.Y;
	}
	else
	{
		CamSwayOffset = FVector2D::ZeroVector;
	}
}

void AGamePlayerBase::InputBinding_Run(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		SetRunState(HasControlFlag(PCF_CanRun) && InValue.Get<bool>());
	}
}

void AGamePlayerBase::InputBinding_Crouch(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		if (HasControlFlag(PCF_CanCrouch) && !IsCrouching())
		{
			SetCrouchState(true);
		}
		else if (!IsStandingBlocked())
		{
			SetCrouchState(false);
		}
	}
}

void AGamePlayerBase::InputBinding_Lean(const FInputActionValue& InValue)
{
	if (CAN_INPUT && !IsValid(LockOnTarget))
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

void AGamePlayerBase::InputBinding_Inventory(const FInputActionValue& InValue)
{
	if (LockFlags.Contains(LockFlag(Guide))) return;
	if (LockFlags.Contains(LockFlag(Inventory)))
	{
		GetInventory()->CloseInventory();
	}
	else if (CAN_INPUT)
	{
		GetInventory()->OpenInventory();
	}
}

void AGamePlayerBase::InputBinding_HideQuests(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
		{
			if (UNarrativeWidgetBase* Widget = WidgetManager->FindWidget<UNarrativeWidgetBase>())
			{
				Widget->SetQuestsHidden(!Widget->AreQuestsHidden());
			}
		}
	}
}

void AGamePlayerBase::InputBinding_Interact(const FInputActionValue& InValue)
{
	Interaction->SetInteracting(CAN_INPUT && HasControlFlag(PCF_CanInteract) && InValue.Get<bool>());
}

void AGamePlayerBase::InputBinding_Equipment(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		GetInventory()->EquipmentUse();
	}
}

void AGamePlayerBase::InputBinding_EquipmentAlt(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		GetInventory()->EquipmentUseAlt(InValue.Get<bool>());
	}
}