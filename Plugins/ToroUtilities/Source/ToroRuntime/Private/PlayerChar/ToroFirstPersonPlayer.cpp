// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerChar/ToroFirstPersonPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

AToroFirstPersonPlayer::AToroFirstPersonPlayer()
	: ReachDistance(250.0f), InteractTrace(ECC_Visibility)
	, Sensitivity(1.0f), SensitivityMulti(1.0f)
	, FieldOfView(90.0f), FieldOfViewSpeed(5.0f)
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
	SensitivityMulti.ClampModifiers(0.0f, 5.0f);
	FieldOfView.ClampModifiers(-25.0f, 25.0f);
#endif
}
