// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayerChar.h"
#include "EnhancedInputComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/InteractionComponent.h"
#include "UserSettings/ToroUserSettings.h"
#include "MusicSystem/GameMusicManager.h"
#include "Inventory/InventoryComponent.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroGameInstance.h"
#include "Libraries/ToroMathLibrary.h"

#define CAN_INPUT !IsLocked() && !IsPaused()
#define TRACE_PARAMS FCollisionQueryParams(NAME_None, false, this)
#define BIND_INPUT_ACTION(Component, Event, Action) \
	if (const UInputAction* IA_##Action = InputActions.FindRef(Player::Inputs::Action)) \
	{\
		Component->BindAction(IA_##Action, ETriggerEvent::Event, this, &ThisClass::InputBinding_##Action); \
	}

AGamePlayerChar::AGamePlayerChar()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
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
	EquipmentRoot->SetRelativeLocation(FVector(22.0f, 20.0f, -15.0f));
	EquipmentRoot->SetupAttachment(PlayerCamera);
#if WITH_EDITORONLY_DATA
	EquipmentRoot->bVisualizeComponent = true;
#endif

	InspectRoot = CreateDefaultSubobject<USceneComponent>("InspectRoot");
	InspectRoot->SetRelativeLocation(FVector(20.0f, -9.0f, 0.0f));
	InspectRoot->SetupAttachment(PlayerCamera);
#if WITH_EDITORONLY_DATA
	InspectRoot->bVisualizeComponent = true;
#endif

	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Interaction = CreateDefaultSubobject<UInteractionComponent>("Interaction");

	PlayerLight->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	
	ControlFlags = Player::DefaultControls;
	StateFlags = 0;
	FOVRunOffset = 5.0f;
	FOVCrouchOffset = -5.0f;
	LockOnSpeed = 5.0f;
	LeanOffsets = FVector2D(75.0f, 25.0f);
	SideTrace = ECC_Visibility;
	StrafeOffsets = FVector2D(2.5f, 1.5f);
	WalkSpeed = 300.0f;
	RunSpeedMulti = 2.5f;
	SneakSpeedMulti = 0.72f;
	CrouchHeights = FVector2D(88.0f, 45.0f);
	CeilingTrace = ECC_Visibility;

	FOV = 88.0f;
	CamOffset = FVector2D::ZeroVector;
	MoveSpeed = 300.0f;
	Sensitivity = FVector2D::UnitVector;
	LeanState = EPlayerLeanState::None;
	InterpFOV = {90.0f, 5.0f};
	InterpCrouch = {88.0f, 5.0f};
	InterpCamOffset = {FVector2D::ZeroVector, 7.5f};
}

bool AGamePlayerChar::IsLocked() const
{
	return !LockTags.IsEmpty() || ControlFlags & PCF_Locked || StateFlags & Player::LockingStates;
}

void AGamePlayerChar::ResetStates()
{
	ExitInspectable();
	ExitHidingSpot();
	ExitWorldDevice();
	ExitActiveTask();
	Inventory->CloseInventory();
}

void AGamePlayerChar::OverrideControlFlags(const int32 InFlags)
{
	for (const EPlayerControlFlags Enum : TEnumRange<EPlayerControlFlags>())
	{
		InFlags & Enum ? SetControlFlag(Enum) : UnsetControlFlag(Enum);
	}
}

void AGamePlayerChar::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None && !HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
		switch (InFlag)
		{
		case PCF_UseStamina: GetWorldTimerManager().UnPauseTimer(StaminaTimer); break;
		default: break;
		}
	}
}

void AGamePlayerChar::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None && HasControlFlag(InFlag))
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

bool AGamePlayerChar::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	return InFlag != PCF_None && ControlFlags & InFlag;
}

void AGamePlayerChar::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None) StateFlags |= InFlag;
}

void AGamePlayerChar::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None) StateFlags &= ~InFlag;
}

bool AGamePlayerChar::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	return InFlag != PSF_None && StateFlags & InFlag;
}

void AGamePlayerChar::SetRunState(const bool bInState)
{
	bool bShouldRun = bInState && !IsCrouching() && !IsStaminaPunished();
	if (HasStateFlag(PSF_Run) != bShouldRun)
	{
		if (bShouldRun)
		{
			SetStateFlag(PSF_Run);
			AddWalkMulti(Player::Keys::Running, RunSpeedMulti);
		}
		else
		{
			UnsetStateFlag(PSF_Run);
			RemoveWalkMulti(Player::Keys::Running);
		}
	}
}

void AGamePlayerChar::SetCrouchState(const bool bInState)
{
	bool bShouldCrouch = bInState && !HasStateFlag(PSF_Run) && !IsStaminaPunished();
	if (IsCrouching() != bShouldCrouch)
	{
		InterpCrouch.Target = bShouldCrouch ? CrouchHeights.Y : CrouchHeights.X;
		if (bShouldCrouch)
		{
			SetStateFlag(PSF_Crouch);
			AddFOVOffset(Player::Keys::Crouching, FOVCrouchOffset);
			AddWalkMulti(Player::Keys::Crouching, SneakSpeedMulti);
		}
		else
		{
			UnsetStateFlag(PSF_Crouch);
			RemoveFOVOffset(Player::Keys::Crouching);
			RemoveWalkMulti(Player::Keys::Crouching);
		}
	}
}

void AGamePlayerChar::SetLeanState(const EPlayerLeanState InState)
{
	if (LeanState == InState) return;
	if (InState == EPlayerLeanState::None)
	{
		LeanState = EPlayerLeanState::None;
		RemoveCamOffset(Player::Keys::Leaning);
		GetWorldTimerManager().PauseTimer(WallDetectTimer);
	}
	else if (const float Dir = Player::LeanToFloat(InState); !IsLeaningBlocked(Dir))
	{
		LeanState = InState;
		AddCamOffset(Player::Keys::Leaning, LeanOffsets * Dir);
		GetWorldTimerManager().UnPauseTimer(WallDetectTimer);
	}
}

void AGamePlayerChar::SetLockOnTarget(const USceneComponent* InComponent)
{
	if (InComponent) LockOnTarget = InComponent;
	else LockOnTarget = nullptr;
}

void AGamePlayerChar::SetFocalDistance(const float InFocalDistance, const float InSpeed)
{
	InterpFocal.Target = FMath::Max(InFocalDistance, 0.0f);
	InterpFocal.Speed = FMath::Max(InSpeed, 0.1f);
}

void AGamePlayerChar::SetInspectable(AActor* InActor)
{
	if (InActor && Inspectable) return; // No hot-swapping
	if (AToroPlayerController* PC = GetPlayerController())
	{
		if (Inspectable && !InActor)
		{
			PC->RemovePauseRequest(Inspectable);
		}
		else if (InActor)
		{
			PC->AddPauseRequest(InActor);
		}
	}

	Inspectable = InActor;
	Inspectable ? SetStateFlag(PSF_Inspect) : UnsetStateFlag(PSF_Inspect);
	SetFocalDistance(Inspectable ? InspectRoot->GetRelativeLocation().X : 100.0f);
}

void AGamePlayerChar::SetHidingSpot(AActor* InActor)
{
	if (InActor && HidingSpot) return; // No hot-swapping
	if (UGameMusicManager* Manager = UGameMusicManager::Get(this))
	{
		if (HidingSpot && !InActor)
		{
			Manager->RemoveDipRequest(HidingSpot);
		}
		else if (InActor)
		{
			Manager->AddDipRequest(InActor);
		}
	}

	HidingSpot = InActor;
	HidingSpot ? SetStateFlag(PSF_Hiding) : UnsetStateFlag(PSF_Hiding);
}

void AGamePlayerChar::SetWorldDevice(AActor* InActor)
{
	WorldDevice = InActor;
	WorldDevice ? SetStateFlag(PSF_Device) : UnsetStateFlag(PSF_Device);
}

void AGamePlayerChar::SetActiveTask(AActor* InActor)
{
	ActiveTask = InActor;
	ActiveTask ? SetStateFlag(PSF_Tasking) : UnsetStateFlag(PSF_Tasking);
}

bool AGamePlayerChar::TryJumpscare()
{
	if (IsPaused() || HasControlFlag(PCF_Locked) || !LockTags.IsEmpty()
		|| GameInstance->IsPlayerInvincible()) return false;

	AddLockTag(PlayerLockTag::TAG_GameOver);
	ResetStates();

	if (UGameMusicManager* Manager = UGameMusicManager::Get(this))
	{
		Manager->RemoveDipRequest(GetPlayerController());
		Manager->RemoveDipRequest(this);
		Manager->SetThemeMuted(true);
	}
	return true;
}

void AGamePlayerChar::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	EquipmentRoot->SetHiddenInGame(bNewHidden, true);
}

void AGamePlayerChar::EnterCinematic(AActor* CinematicActor)
{
	ResetStates();
	Super::EnterCinematic(CinematicActor);
}

bool AGamePlayerChar::GetLookTarget_Implementation(FVector& Location) const
{
	Location = LockOnTarget ? LockOnTarget->GetComponentLocation() : FVector::ZeroVector;
	return IsValid(LockOnTarget);
}

void AGamePlayerChar::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = PlayerCamera->GetComponentLocation();
	Forward = PlayerCamera->GetForwardVector();
	Angle = PlayerCamera->FieldOfView;
}

void AGamePlayerChar::Teleport(const FVector& InLocation, const FRotator& InRotation)
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

void AGamePlayerChar::TickStamina()
{
	Stamina.TickStamina(IsRunning());
	if (Stamina.IsEmpty() && !IsStaminaPunished())
	{
		SetRunState(false);
		SetStateFlag(PSF_RunLocked);
	}
	else if (Stamina.IsFull() && IsStaminaPunished())
	{
		UnsetStateFlag(PSF_RunLocked);
	}
}

void AGamePlayerChar::TickFootstep()
{
	FVector Start, End;
	UToroMathLibrary::GetComponentLineTraceVectors(FootstepAudio, EVectorDirection::Up, -150.0f, Start, End);
	FCollisionQueryParams Params{NAME_None, true, this};
	Params.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Footsteps.TraceChannel, Params);
	if (IsMoving() && Hit.bBlockingHit)
	{
		if (USoundBase* Sound = Footsteps.GetFootstepSound(StateFlags, !Hit.PhysMaterial.IsValid()
			? SurfaceType_Default : UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get())))
		{
			FootstepAudio->SetSound(Sound);
			FootstepAudio->Play();
		}
	}

	FootstepTimer.Invalidate();
}

void AGamePlayerChar::LeanWallDetect()
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

bool AGamePlayerChar::IsStandingBlocked() const
{
	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Up,
		GetCapsuleComponent()->GetScaledCapsuleRadius() + CrouchHeights.X + 5.0f, Start, End);

	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		CeilingTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS);
}

bool AGamePlayerChar::IsLeaningBlocked(const float Direction) const
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

void AGamePlayerChar::OnSettingsChange(const UToroUserSettings* InSettings)
{
	if (InSettings)
	{
		CameraArm->bEnableCameraRotationLag = InSettings->GetSmoothCamera();
		Sensitivity.Base.X = InSettings->GetSensitivityX();
		Sensitivity.Base.Y = InSettings->GetSensitivityY();
	}
}

void AGamePlayerChar::SlowTick()
{
	if (IsRunning())
	{
		if (!HasFOVOffset(Player::Keys::Running))
		{
			AddFOVOffset(Player::Keys::Running, FOVRunOffset);
		}
	}
	else if (HasFOVOffset(Player::Keys::Running))
	{
		RemoveFOVOffset(Player::Keys::Running);
	}

	InterpFOV.Target = FOV.Evaluate();

	MoveSpeed.Base = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.Evaluate();
}

void AGamePlayerChar::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = UToroGameInstance::Get(this);
	if (const AToroCameraManager* CamManager = AToroCameraManager::Get(this))
	{
		EquipmentRoot->AttachToComponent(CamManager->GetTransformComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnDynamicSettingsChanged.AddUObject(this, &AGamePlayerChar::OnSettingsChange);
		OnSettingsChange(Settings);
	}

	InterpCrouch.Target = CrouchHeights.X;
	InterpCrouch.SnapToTarget();
	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpCrouch.Current);

	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(WallDetectTimer, this, &AGamePlayerChar::LeanWallDetect, 0.1f, true);
	Timer.PauseTimer(WallDetectTimer);
	
	Timer.SetTimer(StaminaTimer, this, &AGamePlayerChar::TickStamina, 0.1f, true);
	if (!HasControlFlag(PCF_UseStamina)) Timer.PauseTimer(StaminaTimer);

	ClearLockTag(PlayerLockTag::TAG_Startup);
}

void AGamePlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!InterpFOV.IsComplete())
	{
		InterpFOV.Tick(DeltaTime);
		PlayerCamera->SetFieldOfView(InterpFOV.Current);
	}

	if (!InterpFocal.IsComplete())
	{
		InterpFocal.Tick(DeltaTime);
		PlayerCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = InterpFocal.Current < 99.0f;
		PlayerCamera->PostProcessSettings.DepthOfFieldFocalDistance = InterpFocal.Current;
	}

	if (!IsPaused())
	{
		if (!InterpCrouch.IsComplete())
		{
			InterpCrouch.Tick(DeltaTime);
			GetCapsuleComponent()->SetCapsuleHalfHeight(InterpCrouch.Current);
		}

		InterpCamOffset.Target = CamOffset.Evaluate();
		InterpCamOffset.Tick(DeltaTime);

		const FRotator ControlRotation = GetControlRotation();
		if (LockOnTarget)
		{
			SetLeanState(EPlayerLeanState::None);
			const FVector TargetVector = LockOnTarget->GetComponentLocation() - PlayerCamera->GetComponentLocation();
			Controller->SetControlRotation(FMath::RInterpTo(ControlRotation,
				FRotationMatrix::MakeFromX(TargetVector).Rotator(), DeltaTime, LockOnSpeed));
		}
		else
		{
			Controller->SetControlRotation(FRotator(ControlRotation.Pitch,
				ControlRotation.Yaw, InterpCamOffset.Current.Y));

			if (IsMoving() && IsPlayerControlled())
			{
				const bool bRunning = IsRunning();
				GetPlayerController()->ClientStartCameraShake(bRunning ? UCamShake_Run::StaticClass()
					: UCamShake_Walk::StaticClass(),1.0f);
			}
		}

		CameraArm->SetRelativeLocation(FVector(0.0f, InterpCamOffset.Current.X,
			GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere()));

		FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f,
			-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere()));

		if (!FootstepTimer.IsValid() && IsMoving())
		{
			GetWorldTimerManager().SetTimer(FootstepTimer, this, &AGamePlayerChar::TickFootstep,
				Footsteps.GetFootstepInterval(StateFlags), false);
		}
	}
}

void AGamePlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

void AGamePlayerChar::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHeights.X);

	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f,
		GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere()));

	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f,
		-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere()));

#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		for (const FName& ActionName : Player::Inputs::All)
		{
			if (!InputActions.Contains(ActionName)) InputActions.Add(ActionName);
		}

		Footsteps.Validate();
	}
#endif
}

void AGamePlayerChar::InputBinding_Pause(const FInputActionValue& InValue)
{
	if (HasStateFlag(PSF_Inventory))
	{
		Inventory->CloseInventory();
		return;
	}

	if (CAN_INPUT)
	{
		SetRunState(false);
		SetLeanState(EPlayerLeanState::None);
		GetPlayerController()->SetGamePaused(true);
	}
}

void AGamePlayerChar::InputBinding_Back(const FInputActionValue& InValue)
{
	if (ExitInspectable()) return;
	if (ExitHidingSpot()) return;
	ExitWorldDevice();
}

void AGamePlayerChar::InputBinding_Turn(const FInputActionValue& InValue)
{
	const FVector2D Axis = InValue.Get<FVector2D>();
	if (Inspectable)
	{
		const FVector2D Multi = Sensitivity.Modifiers.FindRef(Player::Keys::Inspecting, FVector2D::UnitVector);
		InspectRoot->AddLocalRotation(FRotator(Axis.Y * Multi.Y, -Axis.X * Multi.X, 0.0f));
		return;
	}
	
	if (CAN_INPUT && HasControlFlag(PCF_CanTurn) && !IsValid(LockOnTarget))
	{
		const FVector2D Multi = Sensitivity.Evaluate();
		if (!FMath::IsNearlyZero(Axis.X))
		{
			AddControllerYawInput(Axis.X * Multi.X);
		}
		if (!FMath::IsNearlyZero(Axis.Y))
		{
			AddControllerPitchInput(Axis.Y * Multi.Y);
		}
	}
}

void AGamePlayerChar::InputBinding_Move(const FInputActionValue& InValue)
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

		AddCamOffset(Player::Keys::Movement, StrafeOffsets * Axis.Y);
	}
	else
	{
		RemoveCamOffset(Player::Keys::Movement);
	}
}

void AGamePlayerChar::InputBinding_Run(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		SetRunState(HasControlFlag(PCF_CanRun) && InValue.Get<bool>());
	}
}

void AGamePlayerChar::InputBinding_Crouch(const FInputActionValue& InValue)
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

void AGamePlayerChar::InputBinding_Lean(const FInputActionValue& InValue)
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

void AGamePlayerChar::InputBinding_Inventory(const FInputActionValue& InValue)
{
	if (StateFlags & (PSF_GuideBook | PSF_QuickTime | PSF_Inspect | PSF_Hiding | PSF_Device)
		|| !LockTags.IsEmpty() || ControlFlags & PCF_Locked) return;
	HasStateFlag(PSF_Inventory) ? Inventory->CloseInventory() : Inventory->OpenInventory();
}

void AGamePlayerChar::InputBinding_HideQuests(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		//Narrative->ToggleQuests(); TODO
	}
}

void AGamePlayerChar::InputBinding_Interact(const FInputActionValue& InValue)
{
	Interaction->SetInteracting(CAN_INPUT && HasControlFlag(PCF_CanInteract) && InValue.Get<bool>());
}

void AGamePlayerChar::InputBinding_Equipment(const FInputActionValue& InValue)
{
	Inventory->SetEquipmentUse(CAN_INPUT && InValue.Get<bool>());
}
