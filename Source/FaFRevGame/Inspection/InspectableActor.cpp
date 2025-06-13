// Copyright (C) RedCraft86. All Rights Reserved.

#include "InspectableActor.h"

AInspectableActor::AInspectableActor()
	: TurningSpeed(1.0, 0.5f), InspectScale(FVector::OneVector), ScaleSpeed(6.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InspectRoot = CreateDefaultSubobject<USpringArmComponent>("InspectRoot");
	InspectRoot->PrimaryComponentTick.bTickEvenWhenPaused = true;
	InspectRoot->SetupAttachment(GetRootComponent());
	InspectRoot->bUsePawnControlRotation = false;
	InspectRoot->bEnableCameraRotationLag = false;
	InspectRoot->CameraRotationLagSpeed = 6.5f;
	InspectRoot->bEnableCameraLag = false;
	InspectRoot->CameraLagSpeed = 5.0f;
	InspectRoot->TargetArmLength = 0.0f;
	InspectRoot->bDoCollisionTest = false;

#if WITH_EDITOR
	UPDATE_VISUAL_MAX_COMP(2)
#endif

	ScaleLerp = {0.0f, 6.0f};
}

void AInspectableActor::BeginInspection(AGamePlayerChar* Player)
{
	if (!PlayerChar)
	{
		GetWorldTimerManager().ClearTimer(LagTimer);
		SetActorTickEnabled(true);

		PlayerChar = Player;
		PlayerChar->SetInspectable(this);
		PlayerChar->AddSensitivityMulti(Player::Keys::Inspecting, TurningSpeed);
		PlayerChar->ResetInspectRotation();

		ScaleLerp.Target = 1.0f;
		ScaleLerp.Speed = ScaleSpeed;
		InspectRoot->bEnableCameraLag = true;
		InspectRoot->bEnableCameraRotationLag = true;
		InspectRoot->SetWorldLocation(PlayerChar->GetInspectLocation());
	}
}

void AInspectableActor::EndInspection()
{
	if (PlayerChar)
	{
		GetWorldTimerManager().SetTimer(LagTimer, this,
			&AInspectableActor::HandleRemoveLag, 2.0f, false);

		ScaleLerp.Target = 0.0f;
		InspectRoot->SetRelativeLocation(FVector::ZeroVector);

		PlayerChar->SetInspectable(nullptr);
		PlayerChar->RemoveSensitivityMulti(Player::Keys::Inspecting);
		PlayerChar->ResetInspectRotation();
		
		PlayerChar = nullptr;
	}
}

void AInspectableActor::HandleRemoveLag()
{
	InspectRoot->bEnableCameraLag = false;
	InspectRoot->bEnableCameraRotationLag = false;
	SetActorTickEnabled(false);
}

void AInspectableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!ScaleLerp.IsComplete())
	{
		ScaleLerp.Tick(DeltaSeconds);
		InspectRoot->SetRelativeScale3D(FMath::Lerp(FVector::OneVector, InspectScale, ScaleLerp.Current));
	}

	InspectRoot->SetRelativeRotation(PlayerChar ? PlayerChar->GetInspectRotation() : FRotator::ZeroRotator);
}

void AInspectableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector::OneVector);
	InspectRoot->SetRelativeTransform(FTransform::Identity);
}
