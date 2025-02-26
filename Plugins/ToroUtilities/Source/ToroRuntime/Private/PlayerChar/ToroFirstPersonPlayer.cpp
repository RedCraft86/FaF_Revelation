// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerChar/ToroFirstPersonPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

AToroFirstPersonPlayer::AToroFirstPersonPlayer() : ReachDistance(250.0f), InteractTrace(ECC_Visibility)
	, FieldOfView(90.0f), FieldOfViewSpeed(5.0f), LockOnSpeed(5.0f), Sensitivity(1.0f), SensitivityMulti(1.0f)
	, LeanOffsets(75.0f, 25.0f), LeanSpeed(7.5f), SideTrace(ECC_Visibility), SideTraceLength(125.0f)
	, MoveSpeedMulti(1.0f), WalkingSpeed(300.0f), SwayOffsets(2.5f, 1.5f), CrouchWalkSpeed(200.0f)
	, CrouchFOV(-5.0f), CeilingTrace(ECC_Visibility), CrouchHeights(88.0f, 45.0f), CrouchRate(1.0f)
	, RunningSpeed(750.0f), RunningFOV(5.0f), MaxStamina(100.0f), StaminaDrainRate(1.0f), StaminaGainRate(1.0f)
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

void AToroFirstPersonPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AToroFirstPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroFirstPersonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
