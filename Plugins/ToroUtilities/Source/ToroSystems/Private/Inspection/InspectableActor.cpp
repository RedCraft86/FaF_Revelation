// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inspection/InspectableActor.h"
#include "Inspection/InspectionComponent.h"

AInspectableActor::AInspectableActor()
	: SecretThreshold(0.4f), InspectScale(0.1f), ScaleSpeed(6.0f), ScaleLerp(0.0f, 6.0f)
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

	SecretAngle = CreateDefaultSubobject<UArrowComponent>("SecretAngle");
	SecretAngle->SetupAttachment(InspectRoot);

#if WITH_EDITOR
	UPDATE_VISUAL_MAX_COMP(2)
#endif

	SecretThreshold = 0.4f;
	ScaleLerp = {0.0f, 6.0f};
}

void AInspectableActor::Exit_Implementation()
{
	if (Manager && Manager->GetInspectable() == this) Manager->EndInspection();
}

void AInspectableActor::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult)
{
	if (!HasValidArchive()) return;
	CamManager = AToroCameraManager::Get(this, 0);
	Manager = UInspectionComponent::Get(this, 0);
	if (Manager)
	{
		ScaleLerp.Target = 1.0f;
		ScaleLerp.Speed = ScaleSpeed;
		InspectRoot->bEnableCameraLag = true;
		InspectRoot->bEnableCameraRotationLag = true;
		InspectRoot->SetWorldLocation(Manager->GetComponentLocation());
		Manager->BeginInspection(this);

		GetWorldTimerManager().ClearTimer(LagTimer);
		SetActorTickEnabled(true);
	}
}

void AInspectableActor::OnEndInspect()
{
	GetWorldTimerManager().SetTimer(LagTimer, this,
		&AInspectableActor::HandleRemoveLag, 2.0f, false);

	ScaleLerp.Target = 0.0f;
	InspectRoot->SetRelativeLocation(FVector::ZeroVector);
	Manager = nullptr;
}

void AInspectableActor::HandleRemoveLag()
{
	InspectRoot->bEnableCameraLag = false;
	InspectRoot->bEnableCameraRotationLag = false;
	SetActorTickEnabled(false);
}

float AInspectableActor::GetSecretDot() const
{
	if (CamManager)
	{
		// Negate the result because we want the arrow to be +1.0 when facing the arrow
		return -FVector::DotProduct(SecretAngle->GetForwardVector(), CamManager->GetActorForwardVector());
	}
	return -1.0; // -1.0 will be opposite of the arrow, never a secret there
}

void AInspectableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!HasValidArchive()) return;
	if (!ScaleLerp.IsComplete())
	{
		ScaleLerp.Tick(DeltaSeconds);
		InspectRoot->SetRelativeScale3D(FMath::Lerp(FVector::OneVector, InspectScale, ScaleLerp.Current));
	}

	if (Manager)
	{
		InspectRoot->SetWorldRotation(Manager->GetComponentRotation());
	}
	else
	{
		InspectRoot->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AInspectableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector::OneVector);
	InspectRoot->SetRelativeTransform(FTransform::Identity);
#if WITH_EDITOR
	SecretAngle->SetVisibility(HasValidArchive() && !Archive->SecretText.IsEmptyOrWhitespace());
#endif
}
