// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inspection/InspectableActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inspection/InspectionManager.h"
#include "Components/ArrowComponent.h"

AInspectableActor::AInspectableActor()
	: SecretThreshold(0.4f), InspectScale(0.1f), ScaleSpeed(6.0f)
	, ScaleInterp(FVector::OneVector, 6.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InspectRoot = CreateDefaultSubobject<USpringArmComponent>("InspectRoot");
	InspectRoot->SetupAttachment(GetRootComponent());
	InspectRoot->SetTickableWhenPaused(true);
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
}

void AInspectableActor::Exit_Implementation()
{
	// TODO link with manager
	// if (Manager && Manager->GetInspectable() == this) Manager->EndInspection();
}

void AInspectableActor::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult)
{
	if (!HasValidArchive()) return;
	Manager = Player->Inspection;
	if (Manager)
	{
		ScaleInterp.Speed = ScaleSpeed;
		ScaleInterp.Target = InspectScale;
		InspectRoot->bEnableCameraLag = true;
		InspectRoot->bEnableCameraRotationLag = true;
		InspectRoot->SetWorldLocation(Manager->GetComponentLocation());

		// TODO link with manager
		// Manager->BeginInspection(this);

		GetWorldTimerManager().ClearTimer(LagTimer);
		SetActorTickEnabled(true);
	}
}

FInteractionInfo AInspectableActor::GetInteractInfo_Implementation(const FHitResult& HitResult)
{
	InteractionInfo.bEnabled = InteractionInfo.bEnabled && HasValidArchive() && GetEnabledState();
	return InteractionInfo;
}

void AInspectableActor::OnEndInspect()
{
	GetWorldTimerManager().SetTimer(LagTimer, this,
		&AInspectableActor::HandleRemoveLag, 2.0f, false);

	ScaleInterp.Target = FVector::OneVector;
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
		// Negate the result because we want the value to be +1.0 when facing the arrow
		return -FVector::DotProduct(SecretAngle->GetForwardVector(), CamManager->GetActorForwardVector());
	}
	return -1.0; // -1.0 will be opposite of the arrow, never a secret there
}

void AInspectableActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (HasValidArchive())
		{
			SetEnabledState(false);
		}
	});
}

void AInspectableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!ScaleInterp.IsComplete())
	{
		InspectRoot->SetRelativeScale3D(ScaleInterp.Tick(DeltaSeconds));
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
	if (Archive && !Archive->IsValidArchive())
	{
		Archive = nullptr;
		return;
	}

	SecretAngle->SetVisibility(HasValidArchive() && !Archive->SecretText.IsEmptyOrWhitespace());
#endif
}
