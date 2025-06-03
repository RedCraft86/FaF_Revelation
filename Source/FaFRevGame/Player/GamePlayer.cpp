// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayer.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UserSettings/ToroUserSettings.h"
#include "MusicSystem/GameMusicManager.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroGameInstance.h"
#include "Libraries/ToroMathLibrary.h"

#define CAN_INPUT !IsLocked() && !IsPaused()
#define TRACE_PARAMS FCollisionQueryParams(NAME_None, false, this)
#define BIND_INPUT_ACTION(Component, Event, Action) \
	if (const UInputAction* IA_##Action = InputActions.FindRef(Player::Inputs::Action)) \
	{ Component->BindAction(IA_##Action, ETriggerEvent::Event, this, &ThisClass::InputBinding_##Action); }

AGamePlayer::AGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	FootstepAudio->SetupAttachment(GetCapsuleComponent());
	FootstepAudio->bAutoActivate = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bDoCollisionTest = false;
	CameraArm->TargetArmLength = 0.0f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>("EquipmentRoot");
#if WITH_EDITORONLY_DATA
	EquipmentRoot->bVisualizeComponent = true;
#endif

	Narrative = CreateDefaultSubobject<UGameNarrative>("Narrative");
	Inventory = CreateDefaultSubobject<UGameInventory>("Inventory");

	ControlFlags = DEFAULT_PLAYER_CONTROL_FLAGS;
	StateFlags = 0;
	FOVValue = 90.0f;
	FOVSpeed = 5.0f;
	FOVRunOffset = 5.0f;
	FOVCrouchOffset = -5.0f;
	LockOnTarget = nullptr;
	LockOnSpeed = 5.0f;
	Sensitivity = FVector2D::UnitVector;
	SensitivityMulti = 1.0f;
	LeanOffsets = FVector2D(75.0f, 25.0f);
	LeanSpeed = 7.5f;
	SideTrace = ECC_Visibility;
	StrafeOffsets = FVector2D(2.5f, 1.5f);
	WalkSpeed = 300.0f;
	RunSpeedMulti = 2.5f;
	SneakSpeedMulti = 0.72f;
	CrouchHeights = FVector2D(88.0f, 45.0f);
	CrouchSpeed = 5.0f;
	CeilingTrace = ECC_Visibility;
	MaxStamina = 100.0f;
	StaminaRates = FVector2D(1.0f, 1.0f);
	FSIntervals = FVector(0.5f, 0.35f, 0.6f);
	FSTrace = ECC_Visibility;
}

void AGamePlayer::ResetStates()
{
	for (auto It = LockFlags.CreateIterator(); It; ++It)
	{
		if (!It->IsValidFlag() || !Player::LockFlags::IsNative(*It) || Player::LockFlags::IsResettable(*It))
		{
			It.RemoveCurrent();
		}
	}
}

void AGamePlayer::OverrideControlFlags(const int32 InFlags)
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

void AGamePlayer::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (!HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
		switch (InFlag)
		{
		case PCF_UseStamina: GetWorldTimerManager().UnPauseTimer(StaminaTimer); break;
		default: break;
		}
	}
}

void AGamePlayer::UnsetControlFlag(const EPlayerControlFlags InFlag)
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
		default: break;
		}
	}
}

bool AGamePlayer::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	if (InFlag == PCF_None) return false;
	return ControlFlags & InFlag;
}

void AGamePlayer::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags |= InFlag;
}

void AGamePlayer::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags &= ~InFlag;
}

bool AGamePlayer::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	if (InFlag == PSF_None) return false;
	return StateFlags & InFlag;
}

void AGamePlayer::SetRunState(const bool bInState)
{
	bool bShouldRun = bInState && !IsCrouching() && !IsStaminaPunished();
	if (HasStateFlag(PSF_Running) != bShouldRun)
	{
		if (bShouldRun)
		{
			SetStateFlag(PSF_Running);
			WalkSpeed.AddMod(Player::Keys::Running, RunSpeedMulti);
		}
		else
		{
			UnsetStateFlag(PSF_Running);
			WalkSpeed.RemoveMod(Player::Keys::Running);
		}
	}
}

void AGamePlayer::SetCrouchState(const bool bInState)
{
	bool bShouldCrouch = bInState && !HasStateFlag(PSF_Running) && !IsStaminaPunished();
	if (IsCrouching() != bShouldCrouch)
	{
		InterpCrouch.Target = bShouldCrouch ? CrouchHeights.Y : CrouchHeights.X;
		if (bShouldCrouch)
		{
			SetStateFlag(PSF_Crouching);
			FOVValue.AddMod(Player::Keys::Crouching, FOVCrouchOffset);
			WalkSpeed.AddMod(Player::Keys::Crouching, SneakSpeedMulti);
		}
		else
		{
			UnsetStateFlag(PSF_Crouching);
			FOVValue.RemoveMod(Player::Keys::Crouching);
			WalkSpeed.RemoveMod(Player::Keys::Crouching);
		}
	}
}

void AGamePlayer::SetLeanState(const EPlayerLeanState InState)
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

void AGamePlayer::SetStaminaPercent(const float InStamina)
{
	Stamina = FMath::Lerp(0, MaxStamina, FMath::Clamp(InStamina, 0.0f, 1.0f));
}

void AGamePlayer::SetLockOnTarget(const USceneComponent* InComponent)
{
	if (InComponent) LockOnTarget = InComponent;
	else LockOnTarget = nullptr;
}

void AGamePlayer::SetHidingSpot(UObject* InObject)
{
	HidingSpot = InObject;
	if (WorldDevice)
	{
		AddPlayerLock(Hiding);
		SetStateFlag(PSF_Hiding);
	}
	else
	{
		ClearPlayerLock(Hiding);
		UnsetStateFlag(PSF_Hiding);
	}
	if (AGameMusicManager* Manager = AGameMusicManager::Get<AGameMusicManager>(this))
	{
		IsValid(HidingSpot) ? Manager->AddDipRequest(this) : Manager->RemoveDipRequest(this);
	}
}

void AGamePlayer::SetWorldDevice(UObject* InObject)
{
	WorldDevice = InObject;
	if (WorldDevice)
	{
		AddPlayerLock(Device);
		SetStateFlag(PSF_Device);
	}
	else
	{
		ClearPlayerLock(Device);
		UnsetStateFlag(PSF_Device);
	}
}

void AGamePlayer::SetTaskDevice(UObject* InObject)
{
	TaskDevice = InObject;
	if (TaskDevice)
	{
		AddPlayerLock(Task);
		SetStateFlag(PSF_Tasking);
	}
	else
	{
		ClearPlayerLock(Task);
		UnsetStateFlag(PSF_Tasking);
	}
}

bool AGamePlayer::TryJumpscare()
{
	if (HasControlFlag(PCF_Locked) || GameInstance->IsPlayerInvincible()) return false;
	for (const FPlayerLockFlag& Flag : LockFlags)
	{
		if (Player::LockFlags::IsImmune(Flag)) return false;
	}

	AddPlayerLock(Jumpscare);

	ExitHiding();
	ExitWorldDevice();
	ExitTaskDevice();
	Inventory->CloseInventory();
	if (AGameMusicManager* Manager = AGameMusicManager::Get<AGameMusicManager>(this))
	{
		Manager->RemoveDipRequest(this);
		Manager->SetThemeMuted(true);
	}
	return true;
}

void AGamePlayer::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	EquipmentRoot->SetHiddenInGame(bNewHidden, true);
}

bool AGamePlayer::GetLookTarget_Implementation(FVector& Location) const
{
	Location = LockOnTarget ? LockOnTarget->GetComponentLocation() : FVector::ZeroVector;
	return IsValid(LockOnTarget);
}

void AGamePlayer::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = PlayerCamera->GetComponentLocation();
	Forward = PlayerCamera->GetForwardVector();
	Angle = PlayerCamera->FieldOfView;
}

void AGamePlayer::Teleport(const FVector& InLocation, const FRotator& InRotation)
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

void AGamePlayer::TickStamina()
{
	const float StaminaDelta = IsRunning() ? -StaminaRates.X : StaminaRates.Y;
	Stamina = FMath::Clamp(Stamina + StaminaDelta, 0.0f, MaxStamina);
	if (FMath::IsNearlyZero(Stamina) && !IsStaminaPunished())
	{
		SetRunState(false);
		SetStateFlag(PSF_RunLocked);
	}
	else if (FMath::IsNearlyEqual(Stamina, MaxStamina, 1.0f) && IsStaminaPunished())
	{
		UnsetStateFlag(PSF_RunLocked);
	}
}

void AGamePlayer::TickFootstep()
{
	FVector Start, End;
	UToroMathLibrary::GetComponentLineTraceVectors(FootstepAudio, EVectorDirection::Up, -150.0f , Start, End);
	FCollisionQueryParams Params{NAME_None, true, this};
	Params.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, FSTrace, Params);
	if (IsMoving() && Hit.bBlockingHit)
	{
		if (USoundBase* Sound = GetFootstepSound(!Hit.PhysMaterial.IsValid() ? SurfaceType_Default
			: UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get())))
		{
			FootstepAudio->SetSound(Sound);
			FootstepAudio->Play();
		}
	}

	FootstepTimer.Invalidate();
}

void AGamePlayer::LeanWallDetect()
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

bool AGamePlayer::IsStandingBlocked() const
{
	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Up,
		GetCapsuleComponent()->GetScaledCapsuleRadius() + CrouchHeights.X + 5.0f, Start, End);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		CeilingTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS);
}

bool AGamePlayer::IsLeaningBlocked(const float Direction) const
{
	if (FMath::IsNearlyZero(Direction)) return false;

	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Right,
		(LeanOffsets.X + 10.0f) * Direction, Start, End);

	Start.Z = End.Z = CameraArm->GetComponentLocation().Z;

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		SideTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS);
}

#define GetFootstep(Type) Type.Find(Surface) ? Type.FindRef(Surface) : Type.FindRef(SurfaceType_Default)
USoundBase* AGamePlayer::GetFootstepSound(const EPhysicalSurface Surface) const
{
	if (IsRunning()) return GetFootstep(FSRun);
	if (IsCrouching()) return GetFootstep(FSSneak);
	return GetFootstep(FSWalk);
}
#undef GetFootstep

float AGamePlayer::GetFootstepInterval() const
{
	if (IsRunning()) return FSIntervals.Y;
	if (IsCrouching()) return FSIntervals.Z;
	return FSIntervals.X;
}

void AGamePlayer::OnSettingsChange(const UToroUserSettings* InSettings)
{
	if (InSettings)
	{
		FOVValue.Base = InSettings->GetFieldOfView();
		PlayerCamera->SetFieldOfView(FMath::Clamp(FOVValue.Evaluate(), 5.0f, 170.0f));
		InterpFOV.Target = PlayerCamera->FieldOfView;
		InterpFOV.SnapToTarget();

		CameraArm->bEnableCameraRotationLag = InSettings->GetSmoothCamera();
		Sensitivity.X = InSettings->GetSensitivityX();
		Sensitivity.Y = InSettings->GetSensitivityY();
	}
}

void AGamePlayer::SlowTick()
{
	if (IsRunning())
	{
		if (FOVValue.HasMod(Player::Keys::Running))
		{
			FOVValue.AddMod(Player::Keys::Running, FOVRunOffset);
		}
	}
	else if (!FOVValue.HasMod(Player::Keys::Running))
	{
		FOVValue.RemoveMod(Player::Keys::Running);
	}
	
	InterpFOV.Target = FOVValue.Evaluate();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed.Evaluate();
}

void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	if (const AToroCameraManager* CamManager = AToroCameraManager::Get(this))
	{
		EquipmentRoot->AttachToComponent(CamManager->GetTransformComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnDynamicSettingsChanged.AddUObject(this, &ThisClass::OnSettingsChange);
		OnSettingsChange(Settings);
	}

	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(WallDetectTimer, this, &ThisClass::LeanWallDetect, 0.1f, true);
	Timer.PauseTimer(WallDetectTimer);
	
	Timer.SetTimer(StaminaTimer, this, &ThisClass::TickStamina, 0.1f, true);
	if (!HasControlFlag(PCF_UseStamina)) Timer.PauseTimer(StaminaTimer);

	ClearPlayerLock(Startup);
}

void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!InterpFOV.IsComplete())
	{
		InterpFOV.Tick(DeltaTime);
		InterpFOV.InterpSpeed = FOVSpeed;
		PlayerCamera->SetFieldOfView(InterpFOV.Current);
	}
	
	if (!InterpCrouch.IsComplete())
	{
		InterpCrouch.Tick(DeltaTime);
		InterpCrouch.InterpSpeed = CrouchSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(InterpCrouch.Current);
	}

	FVector CamPosition = FVector::ZeroVector;
	if (LockOnTarget)
	{
		SetLeanState(EPlayerLeanState::None);
		const FVector TargetVector = LockOnTarget->GetComponentLocation() - PlayerCamera->GetComponentLocation();
		Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(),
			FRotationMatrix::MakeFromX(TargetVector).Rotator(), DeltaTime, LockOnSpeed));
	}
	else
	{
		InterpCamOffset.Target = FVector(CamLeanOffset + CamStrafeOffset, 0.f);
		if (!InterpCamOffset.IsComplete())
		{
			InterpCamOffset.Tick(DeltaTime);
			InterpCamOffset.InterpSpeed = LeanSpeed;
			CamPosition.Y = InterpCamOffset.Current.X;

			const FRotator ControlRotation = GetControlRotation();
			Controller->SetControlRotation(FRotator(ControlRotation.Pitch,
				ControlRotation.Yaw, InterpCamOffset.Current.Y));
		}
		
		if (IsMoving() && IsPlayerControlled())
		{
			const bool bRunning = IsRunning();
			GetPlayerController()->ClientStartCameraShake(bRunning ? UCamShake_Run::StaticClass()
				: UCamShake_Walk::StaticClass(),1.0f);
		}
	}

	const float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f, -HalfHeight));

	const float CamHeight = HalfHeight + GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.5f;
	CamPosition.Z = FMath::FInterpTo(CamPosition.Z, CamHeight, DeltaTime, 10.0f);

	CameraArm->SetRelativeLocation(CamPosition);

	if (!FootstepTimer.IsValid() && IsMoving())
	{
		GetWorldTimerManager().SetTimer(FootstepTimer, this,
			&AGamePlayer::TickFootstep, GetFootstepInterval(), false);
	}
}

void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Pause)
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
		BIND_INPUT_ACTION(EnhancedInputComponent, Started, Equipment)
		BIND_INPUT_ACTION(EnhancedInputComponent, Started, EquipmentAlt)
		BIND_INPUT_ACTION(EnhancedInputComponent, Completed, EquipmentAlt)
		BIND_INPUT_ACTION(EnhancedInputComponent, Canceled, EquipmentAlt)
	}
}

void AGamePlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		for (const FName& ActionName : Player::Inputs::All)
		{
			if (!InputActions.Contains(ActionName)) InputActions.Add(ActionName);
		}

		if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
		{
			for(int32 i = 0; i < Enum->NumEnums(); i++)
			{
				const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
				if (Type != SurfaceType_Default && Enum->HasMetaData(TEXT("Hidden"), i))
				{
					FSWalk.Remove(TEnumAsByte(Type));
					FSRun.Remove(TEnumAsByte(Type));
					FSSneak.Remove(TEnumAsByte(Type));
					continue;
				}
				
				if (!FSWalk.Contains(TEnumAsByte(Type))) FSWalk.Add(TEnumAsByte(Type));
				if (!FSRun.Contains(TEnumAsByte(Type))) FSRun.Add(TEnumAsByte(Type));
				if (!FSSneak.Contains(TEnumAsByte(Type))) FSSneak.Add(TEnumAsByte(Type));
			}
		}
	}
#endif
}

void AGamePlayer::InputBinding_Pause(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		if (LockFlags.Contains(LockFlag(Guide)) || IsValid(GetPlayerController()->GetCinematicActor())) return;
		if (LockFlags.Contains(LockFlag(Inventory)))
		{
			Inventory->CloseInventory();
			return;
		}

		if (IsPaused()) return;
		if (LockFlags.Contains(LockFlag(Device)))
		{
			ExitWorldDevice();
			return;
		}

		if (!IsLocked())
		{
			SetRunState(false);
			SetLeanState(EPlayerLeanState::None);
			GetPlayerController()->SetGamePaused(true);
			if (AGameMusicManager* Manager = AGameMusicManager::Get<AGameMusicManager>(this))
			{
				Manager->AddDipRequest(GetPlayerController());
			}
		}
	}
}

void AGamePlayer::InputBinding_Turn(const FInputActionValue& InValue)
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

void AGamePlayer::InputBinding_Move(const FInputActionValue& InValue)
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

		CamStrafeOffset = StrafeOffsets * Axis.Y;
	}
	else
	{
		CamStrafeOffset = FVector2D::ZeroVector;
	}
}

void AGamePlayer::InputBinding_Run(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		SetRunState(HasControlFlag(PCF_CanRun) && InValue.Get<bool>());
	}
}

void AGamePlayer::InputBinding_Crouch(const FInputActionValue& InValue)
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

void AGamePlayer::InputBinding_Lean(const FInputActionValue& InValue)
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

void AGamePlayer::InputBinding_Inventory(const FInputActionValue& InValue)
{
	if (LockFlags.Contains(LockFlag(Guide))) return;
	if (LockFlags.Contains(LockFlag(Inventory)))
	{
		Inventory->CloseInventory();
	}
	else if (CAN_INPUT)
	{
		Inventory->OpenInventory();
	}
}

void AGamePlayer::InputBinding_HideQuests(const FInputActionValue& InValue)
{
	// TODO
}

void AGamePlayer::InputBinding_Interact(const FInputActionValue& InValue)
{
}

void AGamePlayer::InputBinding_Equipment(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		Inventory->EquipmentUse();
	}
}

void AGamePlayer::InputBinding_EquipmentAlt(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		Inventory->EquipmentUseAlt(InValue.Get<bool>());
	}
}
