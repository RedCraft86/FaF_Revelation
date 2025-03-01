// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerChar/ToroFirstPersonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Framework/ToroPlayerController.h"
#include "UserSettings/ToroUserSettings.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/GameWidgetBase.h"
#include "Camera/CameraComponent.h"
#include "ToroMathLibrary.h"

#define CAN_INPUT !IsLocked() && !IsPaused()
#define TRACE_PARAMS FCollisionQueryParams(NAME_None, false, this)

AToroFirstPersonPlayer::AToroFirstPersonPlayer() : ReachDistance(250.0f), InteractTrace(ECC_Visibility)
	, FieldOfView(90.0f), FieldOfViewSpeed(5.0f), LockOnSpeed(5.0f), Sensitivity(1.0f)
	, SensitivityMulti(1.0f), LeanOffsets(75.0f, 25.0f), LeanSpeed(7.5f), SideTrace(ECC_Visibility)
	, SideTraceLength(125.0f), MoveSpeedMulti(1.0f), WalkingSpeed(300.0f), SwayOffsets(2.5f, 1.5f)
	, CrouchWalkSpeed(200.0f), CrouchFOV(-5.0f), CeilingTrace(ECC_Visibility), CrouchHeights(88.0f, 45.0f)
	, CrouchSpeed(5.0f), RunningSpeed(750.0f), RunningFOV(5.0f), MaxStamina(100.0f), StaminaDrainRate(1.0f)
	, StaminaGainRate(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;

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
	EquipmentRoot->SetupAttachment(PlayerCamera);
#if WITH_EDITOR
	EquipmentRoot->bVisualizeComponent = true;
#endif
	
	Interaction = CreateDefaultSubobject<UInteractionComponent>("Interaction");

	if (FRichCurve* Curve = CrouchAnim.Curve.GetRichCurve())
	{
		FRichCurveKey& Start = Curve->GetKey(Curve->UpdateOrAddKey(0.0f, 1.0f));
		Start.InterpMode = RCIM_Cubic;
		Start.TangentMode = RCTM_Auto;
		Start.ArriveTangent = 0.2;
		Start.LeaveTangent = 1.6;
		
		FRichCurveKey& End = Curve->GetKey(Curve->UpdateOrAddKey(0.5f, 0.0f));
		End.InterpMode = RCIM_Cubic;
		End.TangentMode = RCTM_Auto;
		End.ArriveTangent = 1.6;
		End.LeaveTangent = -0.2;
	}
}

void AToroFirstPersonPlayer::ResetStates()
{
	ClearEnemyStack();
	SetWorldDevice(nullptr);
	SetLockOnTarget(nullptr);
	
	SetRunState(false);
	SetCrouchState(false);
	SetLeanState(EPlayerLeanState::None);

	ExitCinematic();
	ForceExitHiding();
	ForceExitWorldDevice();
	// GameMode->Inventory->CloseUI(); TODO: Inventory close

	LockFlags.Remove(NAME_None);
	const TSet<FName> AllFlags = Player::LockFlags::GetAll();
	const TSet<FName> Resettable = Player::LockFlags::Resettable();
	for (auto It = LockFlags.CreateIterator(); It; ++It)
	{
		if (!AllFlags.Contains(*It) || Resettable.Contains(*It))
		{
			It.RemoveCurrent();
		}
	}

	ControlFlags = DEFAULT_PLAYER_CONTROL_FLAGS;
	MoveSpeedMulti.Modifiers.Empty();
	FieldOfView.Modifiers.Empty();
	InterpFieldOfView.Target = FieldOfView.Evaluate();
	InterpFieldOfView.SnapToTarget();
	InterpHeight.SnapToTarget();
	CamSwayOffset = FVector2D::ZeroVector;
	StaminaDrainRate.Modifiers.Empty();
	StaminaGainRate.Modifiers.Empty();
	CurrentStamina = MaxStamina;
}

void AToroFirstPersonPlayer::SetRunState(const bool bInState)
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

void AToroFirstPersonPlayer::SetCrouchState(const bool bInState)
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

		InterpHeight.Target = bCrouching ? CrouchHeights.Y : CrouchHeights.X;
		if (!bRunning)
		{
			MoveSpeedTarget = bCrouching ? CrouchWalkSpeed : WalkingSpeed;
		}
	}
}

void AToroFirstPersonPlayer::SetLeanState(const EPlayerLeanState InState)
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

void AToroFirstPersonPlayer::SetStaminaPercent(const float InStamina)
{
	CurrentStamina = FMath::Lerp(0, MaxStamina, FMath::Clamp(InStamina, 0.0f, 1.0f));
}

void AToroFirstPersonPlayer::SetLockOnTarget(const USceneComponent* InComponent)
{
	if (InComponent) LockOnTarget = InComponent;
	else LockOnTarget = nullptr;
}

void AToroFirstPersonPlayer::SetHidingSpot(UObject* InObject)
{
	HidingSpot = InObject;
	if (WorldDevice) LockFlags.Add(Tag_LockHiding.GetTag().GetTagName());
	else LockFlags.Remove(Tag_LockHiding.GetTag().GetTagName());
}

void AToroFirstPersonPlayer::ForceExitHiding() const
{
}

void AToroFirstPersonPlayer::SetWorldDevice(UObject* InObject)
{
	WorldDevice = InObject;
	if (WorldDevice) LockFlags.Add(Tag_LockDevice.GetTag().GetTagName());
	else LockFlags.Remove(Tag_LockDevice.GetTag().GetTagName());
}

void AToroFirstPersonPlayer::ForceExitWorldDevice() const
{
}

void AToroFirstPersonPlayer::SetTaskDevice(UObject* InObject)
{
	TaskDevice = InObject;
	if (TaskDevice) SetStateFlag(PSF_Tasking);
	else UnsetStateFlag(PSF_Tasking);
}

void AToroFirstPersonPlayer::ForceExitTaskDevice() const
{
}

void AToroFirstPersonPlayer::AddEnemy(ACharacter* InEnemy)
{
	if (!EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Add(InEnemy);
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AToroFirstPersonPlayer::RemoveEnemy(ACharacter* InEnemy)
{
	if (EnemyStack.Contains(InEnemy))
	{
		EnemyStack.Remove(InEnemy);
		EnemyStack.Remove(nullptr);
		OnEnemyStackChanged();
	}
}

void AToroFirstPersonPlayer::ClearEnemyStack()
{
	if (!EnemyStack.IsEmpty())
	{
		EnemyStack.Empty();
		OnEnemyStackChanged();
	}
}

bool AToroFirstPersonPlayer::TryJumpscare()
{
	if (ControlFlags & PCF_Locked || GameInstance->IsDeveloperMode()) return false;
	const TSet<FName> ImmunityFlags = Player::LockFlags::Immunity();
	for (const FName& Flag : LockFlags)
	{
		if (ImmunityFlags.Contains(Flag)) return false;
	}

	AddLockFlag(Tag_LockJumpscare.GetTag());
	
	ForceExitWorldDevice();
	if (LockFlags.Contains(Tag_LockInventory.GetTag().GetTagName()))
	{
		// TODO: Exit inventory
		//GameMode->Inventory->CloseUI();
	}

	return true;
}

void AToroFirstPersonPlayer::FadeToBlack(const float InTime, const bool bAudio) const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		if (FMath::IsNearlyZero(InTime))
		{
			PlayerController->PlayerCameraManager->SetManualCameraFade(1.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
			   InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroFirstPersonPlayer::FadeFromBlack(const float InTime, const bool bAudio) const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		if (FMath::IsNearlyZero(InTime))
		{
			PlayerController->PlayerCameraManager->SetManualCameraFade(0.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f,
			   InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroFirstPersonPlayer::ClearFade() const
{
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		PlayerController->PlayerCameraManager->StopCameraFade();
	}
}

void AToroFirstPersonPlayer::SetControlFlag(const EPlayerControlFlags InFlag)
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

void AToroFirstPersonPlayer::UnsetControlFlag(const EPlayerControlFlags InFlag)
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

void AToroFirstPersonPlayer::TeleportPlayer(const FVector& InLocation, const FRotator& InRotation)
{
	PlayerController->PlayerCameraManager->SetGameCameraCutThisFrame();
	SetActorLocation(InLocation, false, nullptr, ETeleportType::ResetPhysics);

	const bool bSmooth = CameraArm->bEnableCameraRotationLag;
	CameraArm->bEnableCameraRotationLag = false;
	FRotator Rot(InRotation); Rot.Roll = 0.0f;
	Controller->SetControlRotation(Rot);
	CameraArm->bEnableCameraRotationLag = bSmooth;
}

void AToroFirstPersonPlayer::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	EquipmentRoot->SetHiddenInGame(bNewHidden, true);
}

bool AToroFirstPersonPlayer::GetLookTarget_Implementation(FVector& Location)
{
	Location = LockOnTarget ? LockOnTarget->GetComponentLocation() : FVector::ZeroVector;
	return IsValid(LockOnTarget);
}

void AToroFirstPersonPlayer::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle)
{
	Location = PlayerCamera->GetComponentLocation();
	Forward = PlayerCamera->GetForwardVector();
	Angle = PlayerCamera->FieldOfView;
}

bool AToroFirstPersonPlayer::IsStandingBlocked() const
{
	FVector Start, End;
	UToroMathLibrary::GetActorLineTraceVectors(this, EVectorDirection::Up,
		GetCapsuleComponent()->GetUnscaledCapsuleRadius() + CrouchHeights.X + 5.0f, Start, End);
	
	FHitResult HitResult;
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
		CeilingTrace, FCollisionShape::MakeSphere(10.0f), TRACE_PARAMS);
}

bool AToroFirstPersonPlayer::IsLeaningBlocked(const float Direction) const
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

void AToroFirstPersonPlayer::TickStamina()
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

void AToroFirstPersonPlayer::TickFootstep()
{
	FVector Start, End;
	UToroMathLibrary::GetComponentLineTraceVectors(FootstepAudio,
		EVectorDirection::Up, -150.0f , Start, End);

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

void AToroFirstPersonPlayer::LeanWallDetect()
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

void AToroFirstPersonPlayer::OnEnemyStackChanged()
{
	// TODO: Music states
}

void AToroFirstPersonPlayer::OnSettingsChange(const UToroUserSettings* InSettings)
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

void AToroFirstPersonPlayer::SlowTick()
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

void AToroFirstPersonPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnDynamicSettingsChanged.AddUObject(this, &ThisClass::OnSettingsChange);
		OnSettingsChange(Settings);
	}

	if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
	{
		GameWidget = WidgetManager->FindWidget<UGameWidgetBase>();
	}

	FTimerManager& Timer = GetWorldTimerManager();
	{
		Timer.SetTimer(SlowTickTimer, this, &ThisClass::SlowTick, SlowTickInterval, true);
	
		Timer.SetTimer(WallDetectTimer, this, &ThisClass::LeanWallDetect, 0.1f, true);
		Timer.PauseTimer(WallDetectTimer);
	
		Timer.SetTimer(StaminaTimer, this, &ThisClass::TickStamina, 0.1f, true);
		if (!HasControlFlag(PCF_UseStamina)) Timer.PauseTimer(StaminaTimer);
	}
}

void AToroFirstPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameWidget)
	{
		GameWidget->UpdateStamina(DeltaTime, HasControlFlag(PCF_UseStamina)
			? GetStaminaPercent() : -1.0f, StaminaDelta, IsStaminaPunished());
	}
}

void AToroFirstPersonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		INPUT_EVENT_BINDS(EIC)
	}
}

void AToroFirstPersonPlayer::OnConstruction(const FTransform& Transform)
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

void AToroFirstPersonPlayer::InputBinding_Pause(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Turn(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Move(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Run(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Crouch(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Lean(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Inventory(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_HideQuests(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Interact(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_Equipment(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}

void AToroFirstPersonPlayer::InputBinding_EquipmentAlt(const FInputActionValue& InValue)
{
	if (CAN_INPUT)
	{
		
	}
}
