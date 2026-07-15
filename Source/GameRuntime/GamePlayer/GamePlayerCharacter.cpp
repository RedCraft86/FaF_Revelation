// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Framework/ToroPlayerCameraManager.h"
#include "Interaction/InteractionManager.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WorldMusic/WorldMusicManager.h"
#include "Interfaces/IGenericExit.h"
#include "DataTypes/CameraShakes.h"
#include "TimerManager.h"
#include "GameRuntime.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Char_Player, "Char.Player")

UE_DEFINE_GAMEPLAY_TAG(TAG_LockFlag, "LockFlag")
UE_DEFINE_GAMEPLAY_TAG(TAG_LockFlag_Cinematic, "LockFlag.Cinematic")
UE_DEFINE_GAMEPLAY_TAG(TAG_LockFlag_Loading, "LockFlag.Loading")

AGamePlayerCharacter::AGamePlayerCharacter()
	: bTeleporting(false), bUsingStamina(true), LeanState(EPlayerLeanState::None)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bDoCollisionTest = false;
	CameraArm->TargetArmLength = 0.0f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	PlayerCamera->SetFieldOfView(88.f);

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("EquipmentRoot"));
	EquipmentRoot->SetRelativeLocation(FVector(22.0f, 20.0f, -15.0f));
	EquipmentRoot->SetupAttachment(PlayerCamera);
#if WITH_EDITORONLY_DATA
	EquipmentRoot->bVisualizeComponent = true;
#endif

	Interaction = CreateDefaultSubobject<UInteractionManager>(TEXT("Interaction"));

	UniqueId = TAG_Char_Player.GetTag();

	ControlFlags = Player::DefaultAbilities;
	StateFlags = PSF_None;
	InteractDistance = 200.0f;
	LockOnSpeed = 5.0f;
	LeanOffsets = FVector2D(75.0f, 25.0f);
	WalkSpeed = 300.0f;
	StrafeOffsets = FVector2D(2.5f, 1.5f);
	RunConfig = FPlayerMovementInfo(2.5f, 5.0f);
	SneakConfig = FPlayerMovementInfo(0.5f, -5.0f);
	CrouchHeights = FVector2D(88.0f, 45.0f);

	LeanState = EPlayerLeanState::None;
	FOVModifiers = FNumericModifierList(88.0f);
	FOVInterp = FNumericValueInterp(88.0f, 5.0f, false);
	CamOffsets = FNumericModifierList(FVector2D::ZeroVector);
	CamOffsetInterp = FNumericValueInterp(FVector2D::ZeroVector, 7.5f, false);
	MoveMultipliers = FNumericModifierList(1.0f);
	TurnMultipliers = FNumericModifierList(FVector2D::UnitVector);
	CrouchInterp = FNumericValueInterp(88.0f, 5.0f, false);
}

void AGamePlayerCharacter::OverrideControlFlags(const int32 InFlags)
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

void AGamePlayerCharacter::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None)
	{
		if (!HasControlFlag(InFlag))
		{
			ControlFlags |= InFlag;
		}
		switch (InFlag)
		{
			case PCF_UseStamina:	SetStaminaEnabled(true);	break;
			case PCF_CanInteract:	Interaction->Activate();	break;
			default: break;
		}
	}
}

void AGamePlayerCharacter::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None)
	{
		if (HasControlFlag(InFlag))
		{
			ControlFlags &= ~InFlag;
		}
		switch (InFlag)
		{
			case PCF_CanRun:		SetRunState(false);						break;
			case PCF_CanCrouch:		SetCrouchState(false);					break;
			case PCF_CanLean:		SetLeanState(EPlayerLeanState::None);	break;
			case PCF_UseStamina:	SetStaminaEnabled(false);				break;
			case PCF_CanInteract:
				Interaction->Deactivate();
				if (OperatingTarget.IsValid())
				{
					IGenericExit::Exit(OperatingTarget.Get(), this);
				}
				break;
			case PCF_CanHide:
				if (HidingSpot.IsValid())
				{
					IGenericExit::Exit(HidingSpot.Get(), this);
				}
				break;
			default: break;
		}
	}
}

bool AGamePlayerCharacter::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	return InFlag != PCF_None && ControlFlags & InFlag;
}

void AGamePlayerCharacter::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None)
	{
		StateFlags |= InFlag;
	}
}

void AGamePlayerCharacter::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None)
	{
		StateFlags &= ~InFlag;
	}
}

bool AGamePlayerCharacter::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	return InFlag != PSF_None && StateFlags & InFlag;
}

void AGamePlayerCharacter::SetLockFlag(const FGameplayTag InFlag)
{
	if (InFlag.IsValid() && InFlag != TAG_LockFlag.GetTag()) // Cannot be root tag
	{
		LockFlags.AddTag(InFlag);
	}
}

void AGamePlayerCharacter::UnsetLockFlag(const FGameplayTag InFlag)
{
	if (InFlag.IsValid() && InFlag != TAG_LockFlag.GetTag()) // Cannot be root tag
	{
		LockFlags.RemoveTag(InFlag);
	}
}

bool AGamePlayerCharacter::HasLockFlag(const FGameplayTag InFlag) const
{
	return InFlag.IsValid() && InFlag != TAG_LockFlag.GetTag() && LockFlags.HasTagExact(InFlag);
}

void AGamePlayerCharacter::SetLockOnActor(const AActor* Target)
{
	if (Target && Target != this)
	{
		LockOnTarget = Target->GetRootComponent();
	}
	else
	{
		ClearLockOn();
	}
}

void AGamePlayerCharacter::SetLockOnComponent(const USceneComponent* Target)
{
	if (Target)
	{
		LockOnTarget = Target;
	}
	else
	{
		ClearLockOn();
	}
}

void AGamePlayerCharacter::ClearLockOn()
{
	LockOnTarget.Reset();
}

void AGamePlayerCharacter::ResetStates(const bool bClearModifiers)
{
	RefillStamina();
	SetRunState(false);
	SetCrouchState(false);
	SetLeanState(EPlayerLeanState::None);
	OnUserSettings(UToroGameUserSettings::Get(), EUserSettingApplyType::Manual);

	if (bClearModifiers)
	{
		FOVModifiers.Clear();
		CamOffsets.Clear();
		MoveMultipliers.Clear();
		TurnMultipliers.Clear();
	}

	Interaction->SetInteracting(false);
	if (HidingSpot.IsValid())
	{
		IGenericExit::Exit(HidingSpot.Get(), this);
	}
	if (OperatingTarget.IsValid())
	{
		IGenericExit::Exit(OperatingTarget.Get(), this);
	}

	PlayerController->FlushPressedKeys();
}

void AGamePlayerCharacter::SetRunState(const bool bInState)
{
	const bool bTargetState = bInState && !Stamina.IsPunished() && !HasStateFlag(PSF_Crouch);
	if (HasStateFlag(PSF_Run) != bTargetState)
	{
		if (bTargetState)
		{
			SetStateFlag(PSF_Run);
			MoveMultipliers.AddOrUpdate(Player::InternalKeys::Running, RunConfig.SpeedMultiplier);
		}
		else
		{
			UnsetStateFlag(PSF_Run);
			MoveMultipliers.Remove(Player::InternalKeys::Running);
		}
	}
}

bool AGamePlayerCharacter::IsRunning() const
{
	return HasStateFlag(PSF_Run) && IsMoving();
}

void AGamePlayerCharacter::SetCrouchState(const bool bInState)
{
	const bool bTargetState = bInState && !HasStateFlag(PSF_Run);
	if (HasStateFlag(PSF_Crouch) != bTargetState)
	{
		if (bTargetState)
		{
			SetStateFlag(PSF_Crouch);
			FOVModifiers.AddOrUpdate(Player::InternalKeys::Crouching, SneakConfig.FOV_Offset);
			MoveMultipliers.AddOrUpdate(Player::InternalKeys::Crouching, SneakConfig.SpeedMultiplier);
			CrouchInterp.SetTarget(CrouchHeights.GetMin());
		}
		else
		{
			UnsetStateFlag(PSF_Crouch);
			FOVModifiers.Remove(Player::InternalKeys::Crouching);
			MoveMultipliers.Remove(Player::InternalKeys::Crouching);
			CrouchInterp.SetTarget(CrouchHeights.GetMax());
		}
	}
}

bool AGamePlayerCharacter::IsSneaking() const
{
	return HasStateFlag(PSF_Crouch) && IsMoving();
}

void AGamePlayerCharacter::SetLeanState(const EPlayerLeanState InState)
{
	if (LeanState == InState)
	{
		return;
	}

	if (InState == EPlayerLeanState::None)
	{
		LeanState = EPlayerLeanState::None;
		CamOffsets.Remove(Player::InternalKeys::Leaning);
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else if (const int32 Dir = Player::LeanToNumber(InState); !IsLeaningBlocked(Dir))
	{
		LeanState = InState;
		CamOffsets.AddOrUpdate(Player::InternalKeys::Leaning, LeanOffsets * Dir);
		GetWorldTimerManager().UnPauseTimer(WallDetectTimer);
	}
}

EPlayerLeanState AGamePlayerCharacter::GetLeanState() const
{
	return LeanState;
}

void AGamePlayerCharacter::SetStaminaEnabled(bool bInEnabled)
{
	bUsingStamina = bInEnabled;
	RefillStamina();
}

bool AGamePlayerCharacter::IsStaminaEnabled() const
{
	return bUsingStamina;
}

void AGamePlayerCharacter::RefillStamina()
{
	Stamina.SetPercent(1.0f);
	OnStaminaUpdate.Broadcast(IsStaminaEnabled(), Stamina);
}

void AGamePlayerCharacter::SetHidingSpot(AActor* InSpot)
{
	if (InSpot)
	{
		HidingSpot = InSpot;
	}
	else
	{
		HidingSpot.Reset();
	}
}

void AGamePlayerCharacter::SetOperatingTarget(AActor* InTarget)
{
	if (InTarget)
	{
		OperatingTarget = InTarget;
	}
	else
	{
		OperatingTarget.Reset();
	}
}

bool AGamePlayerCharacter::TryKillPlayer(const FGameplayTag& FromEnemy)
{
	if (IsKillLocked())
	{
		return false;
	}

	ResetStates();
	KillEnemy = FromEnemy;
	OnKilled.Broadcast(KillEnemy);

	if (UWorldMusicManager* MusicManager = UWorldMusicManager::Get(this))
	{
		MusicManager->ClearDipRequests();
		MusicManager->FadeOut(0.5f, 0.0f);
	}

	return true;
}

bool AGamePlayerCharacter::IsLocked() const
{
	return IsKillLocked()
		|| StateFlags & PSF_Hide
		|| StateFlags & PSF_Operate;
}

bool AGamePlayerCharacter::IsKillLocked() const
{
	return IsPaused()
		|| KillEnemy.IsValid()
		|| !LockFlags.IsEmpty()
		|| StateFlags & PSF_Tutorial
		|| StateFlags & PSF_Dialogue;
}

FVector AGamePlayerCharacter::GetFocusPoint_Implementation() const
{
	return GetActorLocation() + FVector(
		0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere()
	);
}

void AGamePlayerCharacter::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = PlayerCamera->GetComponentLocation();
	Forward = PlayerCamera->GetForwardVector();
	Angle = PlayerCamera->FieldOfView;
}

bool AGamePlayerCharacter::GetLookPoint_Implementation(FVector& Location) const
{
	if (LockOnTarget.IsValid())
	{
		Location = LockOnTarget->GetComponentLocation();
		return true;
	}

	Location = FVector::ZeroVector;
	return false;
}

void AGamePlayerCharacter::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	EquipmentRoot->SetHiddenInGame(bNewHidden, true);
}

void AGamePlayerCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	if (bTeleporting)
	{
		UE_LOG(LogGameRuntime, Error, TEXT("Player is already teleporting!"));
	}
	else
	{
		AsyncTeleport(InLocation, InRotation);
	}
}

bool AGamePlayerCharacter::IsStandingBlocked() const
{
	const float Distance = GetCapsuleComponent()->GetScaledCapsuleRadius() + CrouchHeights.GetMax() + 5.0f;

	const FVector Start = GetActorLocation();
	const FVector End = Start + (GetActorUpVector() * Distance);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		ECC_Visibility, FCollisionShape::MakeSphere(10.0f), FCollisionQueryParams(NAME_None, false, this));
}

AActor* AGamePlayerCharacter::HandleInteraction() const
{
	const AGamePlayerController* PlayerCont = const_cast<AGamePlayerCharacter*>(this)->PlayerController.Get();
	if (PlayerCont->bCinematicMode)
	{
		return nullptr;
	}

	FHitResult HitResult;
	if (PlayerCont->GetViewTarget() == this)
	{
		const FVector Start = PlayerCamera->GetComponentLocation();
		const FVector End = Start + (GetActorForwardVector() * InteractDistance);
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, 
			ECC_Visibility, FCollisionQueryParams(NAME_None, false, this));
	}
	else if (PlayerCont->ShouldShowMouseCursor())
	{
		PlayerCont->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	}

	return HitResult.GetActor();
}

bool AGamePlayerCharacter::IsLeaningBlocked(const int32 Dir) const
{
	if (Dir == 0)
	{
		return false;
	}

	const float Distance = (LeanOffsets.X + 10.0f) * FMath::Clamp(Dir, -1, 1);

	FVector Start = GetActorLocation(); 
	Start.Z = PlayerCamera->GetComponentLocation().Z;
	const FVector End = Start + (GetActorRightVector() * Distance);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		ECC_Visibility, FCollisionShape::MakeSphere(10.0f), FCollisionQueryParams(NAME_None, false, this));
}

UE5Coro::TCoroutine<> AGamePlayerCharacter::AsyncTeleport(const FVector& InLocation, const FRotator& InRotation)
{
	bTeleporting = true;
	const bool bSmooth = CameraArm->bEnableCameraRotationLag;
	CameraArm->bEnableCameraRotationLag = false;

	co_await UE5Coro::Latent::NextTick();

	Super::Teleport(InLocation, InRotation);

	co_await UE5Coro::Latent::NextTick();

	CameraArm->bEnableCameraRotationLag = bSmooth;
	bTeleporting = false;
}

void AGamePlayerCharacter::TickFootstep()
{
	TEnumAsByte<EPhysicalSurface> Surface;
	if (IsMoving() && GetFloorSurface(Surface, Footsteps.TraceChannel))
	{
		float Volume = Footsteps.Volume;
		if (IsSneaking())
		{
			Volume *= 0.25f;
		}
		if (IsRunning())
		{
			Volume *= 2.0f;
		}

		PlayFootstep(Footsteps.GetFootstepSound(Surface), Volume);
	}

	FootstepTimer.Invalidate();
}

void AGamePlayerCharacter::TickWallDetect()
{
	if (LeanState == EPlayerLeanState::None)
	{
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else if (IsLeaningBlocked(Player::LeanToNumber(LeanState)))
	{
		SetLeanState(EPlayerLeanState::None);
	}
}

void AGamePlayerCharacter::OnUserSettings(const UToroGameUserSettings* Settings, const EUserSettingApplyType ApplyType)
{
	if (ApplyType != EUserSettingApplyType::UIRefresh)
	{
		CameraArm->bEnableCameraRotationLag = Settings->GetSmoothCamera();
		TurnMultipliers.AddOrUpdate(Player::InternalKeys::Sensitivity, Settings->GetSensitivityMultiplier());
	}
}

void AGamePlayerCharacter::OnCinematic(const bool bCinematicMode)
{
	if (bCinematicMode)
	{
		ResetStates();
		LockFlags.AddTag(TAG_LockFlag_Cinematic.GetTag());
	}
	else
	{
		LockFlags.RemoveTag(TAG_LockFlag_Cinematic.GetTag());
	}
}

void AGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UToroGameUserSettings* UserSettings = UToroGameUserSettings::Get())
	{
		OnUserSettings(UserSettings, EUserSettingApplyType::Manual);
		UserSettings->OnSettingsApplied.AddUObject(this, &AGamePlayerCharacter::OnUserSettings);
	}

	LockFlags.AddTag(TAG_LockFlag_Loading.GetTag());
	Interaction->ExecuteTraceLogic.BindUObject(this, &AGamePlayerCharacter::HandleInteraction);
	Interaction->SetActive(HasControlFlag(PCF_CanInteract));

	if (!HasControlFlag(PCF_UseStamina))
	{
		SetStaminaEnabled(false);
	}

	CrouchInterp.SetTarget(CrouchHeights.GetMax()); 
	CrouchInterp.SnapToTarget();

	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchInterp.GetValue());

	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(WallDetectTimer, this, &AGamePlayerCharacter::TickWallDetect, 0.1f, true);
	Timer.PauseTimer(WallDetectTimer);
	
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		PlayerController->SetViewTarget(this);
		if (const AToroPlayerCameraManager* CamManager = AToroPlayerCameraManager::Get(this))
		{
			EquipmentRoot->AttachToComponent(CamManager->GetTransformComponent(), 
				FAttachmentTransformRules::KeepRelativeTransform);
		}
	});
}

void AGamePlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UToroGameUserSettings::Get()->OnSettingsApplied.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void AGamePlayerCharacter::SlowTick()
{
	Super::SlowTick();

	FOVInterp.SetTarget(FOVModifiers.GetSum());
	CamOffsetInterp.SetTarget(CamOffsets.GetSum());
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * MoveMultipliers.GetProduct();

	if (IsRunning())
	{
		if (!FOVModifiers.Contains(Player::InternalKeys::Running))
		{
			FOVModifiers.AddOrUpdate(Player::InternalKeys::Running, RunConfig.FOV_Offset);
		}
	}
	else if (FOVModifiers.Contains(Player::InternalKeys::Running))
	{
		FOVModifiers.Remove(Player::InternalKeys::Running);
	}
}

void AGamePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const bool bIsMovingThisFrame = IsMoving();

	if (bUsingStamina && Stamina.TickStamina(DeltaTime, bIsMovingThisFrame && HasStateFlag(PSF_Run)))
	{
		OnStaminaUpdate.Broadcast(true, Stamina);
		if (HasStateFlag(PSF_Run) && Stamina.IsEmpty())
		{
			SetRunState(false);
		}
	}

	if (FOVInterp.TickInterp(DeltaTime))
	{
		PlayerCamera->SetFieldOfView(FOVInterp.GetValue());
	}

	if (CrouchInterp.TickInterp(DeltaTime))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchInterp.GetValue());
	}

	CamOffsetInterp.TickInterp(DeltaTime);
	if (LockOnTarget.IsValid())
	{
		FVector LockTarget;
		GetLookPoint(this, LockTarget);
		SetControlRotation(FMath::RInterpTo(GetControlRotation(), 
			FRotationMatrix::MakeFromX(LockTarget - PlayerCamera->GetComponentLocation()).Rotator(),
			DeltaTime, FMath::Max(0.1f, LockOnSpeed)));
	}
	else
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Roll = CamOffsetInterp.GetValue().Y;
		SetControlRotation(ControlRot);

		if (bIsMovingThisFrame && HasStateFlag(PSF_Run))
		{
			PlayerController->ClientStartCameraShake(UCamShake_Run::StaticClass(), 1.0f);
		}
		else if (bIsMovingThisFrame)
		{
			PlayerController->ClientStartCameraShake(UCamShake_Walk::StaticClass(), 1.0f);
		}
	}

	CameraArm->SetRelativeLocation(FVector(
		0.0f, CamOffsetInterp.GetValue().X, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere()
	));

	if (!FootstepTimer.IsValid() && bIsMovingThisFrame)
	{
		GetWorldTimerManager().SetTimer(FootstepTimer,
			this, &AGamePlayerCharacter::TickFootstep,
			Footsteps.GetFootstepInterval(StateFlags), false);
	}
}

void AGamePlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CameraArm->SetRelativeLocation(FVector(
		0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere()
	));

#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		Footsteps.AllocateSlots();
		for (const FName& ActionName : Player::InputKeys::All)
		{
			if (!InputActions.Contains(ActionName))
			{
				InputActions.Add(ActionName);
			}
		}
	}
#endif
}

#define BIND_INPUT_ACTION(Event, Action) \
	if (const UInputAction* IA_##Action = InputActions.FindRef(Player::InputKeys::Action)) \
	{\
		EnhancedInputComponent->BindAction(IA_##Action, ETriggerEvent::Event, this, &AGamePlayerCharacter::InputBinding_##Action); \
	}
void AGamePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BIND_INPUT_ACTION(Started, Pause)

		BIND_INPUT_ACTION(Started, Back)

		BIND_INPUT_ACTION(Triggered, Turn)

		BIND_INPUT_ACTION(Triggered, Move)
		BIND_INPUT_ACTION(Completed, Move)
		BIND_INPUT_ACTION(Canceled, Move)

		BIND_INPUT_ACTION(Started, Run)
		BIND_INPUT_ACTION(Completed, Run)
		BIND_INPUT_ACTION(Canceled, Run)

		BIND_INPUT_ACTION(Started, Crouch)

		BIND_INPUT_ACTION(Started, Lean)
		BIND_INPUT_ACTION(Completed, Lean)
		BIND_INPUT_ACTION(Canceled, Lean)

		BIND_INPUT_ACTION(Triggered, Interact)
		BIND_INPUT_ACTION(Completed, Interact)
		BIND_INPUT_ACTION(Canceled, Interact)

		BIND_INPUT_ACTION(Started, Equipment)
	}
}
#undef BIND_INPUT_ACTION

void AGamePlayerCharacter::InputBinding_Pause(const FInputActionValue& InValue)
{
	if (!IsLocked())
	{
		SetRunState(false);
		SetCrouchState(false);
		SetLeanState(EPlayerLeanState::None);
		Interaction->SetInteracting(false);
		PlayerController->SetGamePaused(true);
	}
}

void AGamePlayerCharacter::InputBinding_Back(const FInputActionValue& InValue)
{
	if (HidingSpot.IsValid())
	{
		IGenericExit::Exit(HidingSpot.Get(), this);
	}

	if (OperatingTarget.IsValid())
	{
		IGenericExit::Exit(OperatingTarget.Get(), this);
	}
}

void AGamePlayerCharacter::InputBinding_Turn(const FInputActionValue& InValue)
{
	if (!IsLocked() && HasControlFlag(PCF_CanTurn) && !LockOnTarget.IsValid())
	{
		const FVector2D Axis = InValue.Get<FVector2D>() * TurnMultipliers.GetProduct();
		AddControllerPitchInput(Axis.Y);
		AddControllerYawInput(Axis.X);
	}
}

void AGamePlayerCharacter::InputBinding_Move(const FInputActionValue& InValue)
{
	if (!IsLocked() && HasControlFlag(PCF_CanMove))
	{
		const FVector2D Axis = InValue.Get<FVector2D>();
		AddMovementInput(GetActorRightVector(), Axis.Y);
		AddMovementInput(GetActorForwardVector(), Axis.X);
		CamOffsets.AddOrUpdate(Player::InternalKeys::Movement, StrafeOffsets * Axis.Y);
	}
	else if (CamOffsets.Contains(Player::InternalKeys::Movement))
	{
		CamOffsets.Remove(Player::InternalKeys::Movement);
	}
}

void AGamePlayerCharacter::InputBinding_Run(const FInputActionValue& InValue)
{
	SetRunState(!IsLocked() && HasControlFlag(PCF_CanRun) && InValue.Get<bool>());
}

void AGamePlayerCharacter::InputBinding_Crouch(const FInputActionValue& InValue)
{
	if (!IsLocked())
	{
		if (HasControlFlag(PCF_CanCrouch) && !HasStateFlag(PSF_Crouch))
		{
			SetCrouchState(true);
		}
		else if (!IsStandingBlocked())
		{
			SetCrouchState(false);
		}
	}
}

void AGamePlayerCharacter::InputBinding_Lean(const FInputActionValue& InValue)
{
	if (!IsLocked() && !LockOnTarget.IsValid())
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

// ReSharper disable once CppMemberFunctionMayBeConst
void AGamePlayerCharacter::InputBinding_Interact(const FInputActionValue& InValue)
{
	Interaction->SetInteracting(!IsLocked() && HasControlFlag(PCF_CanInteract) && InValue.Get<bool>());
}

void AGamePlayerCharacter::InputBinding_Equipment(const FInputActionValue& InValue)
{
	if (!IsLocked())
	{
		InventoryManager->UseEquipment();
	}
}
