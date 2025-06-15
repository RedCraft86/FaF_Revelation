// Copyright (C) RedCraft86. All Rights Reserved.

#include "InspectableActor.h"
#include "Components/ArrowComponent.h"
#include "Inventory/InventoryComponent.h"

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

	SecretAngle = CreateDefaultSubobject<UArrowComponent>("SecretAngle");
	SecretAngle->SetupAttachment(InspectRoot);

#if WITH_EDITOR
	UPDATE_VISUAL_MAX_COMP(2)
#endif

	MinSecretDotAngle = 0.4f;
	ScaleLerp = {0.0f, 6.0f};
}

void AInspectableActor::Exit_Implementation()
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

void AInspectableActor::OnBeginInteract_Implementation(AGamePlayerChar* Player, const FHitResult& HitResult)
{
	if (!PlayerChar)
	{
		GetWorldTimerManager().ClearTimer(LagTimer);
		SetActorTickEnabled(true);

		PlayerChar = Player;
		PlayerChar->ResetInspectRotation();
		PlayerChar->SetInspectable(this);
		PlayerChar->AddSensitivityMulti(Player::Keys::Inspecting, TurningSpeed);

		ScaleLerp.Target = 1.0f;
		ScaleLerp.Speed = ScaleSpeed;
		InspectRoot->bEnableCameraLag = true;
		InspectRoot->bEnableCameraRotationLag = true;
		InspectRoot->SetWorldLocation(PlayerChar->GetInspectLocation());
	}
}

void AInspectableActor::HandleRemoveLag()
{
	InspectRoot->bEnableCameraLag = false;
	InspectRoot->bEnableCameraRotationLag = false;
	SetActorTickEnabled(false);
}

float AInspectableActor::GetSecretDotAngle() const
{
	if (PlayerChar)
	{
		// Negate the result because we want the arrow to be +1.0 when facing the arrow
		return -FVector::DotProduct(SecretAngle->GetForwardVector(),
			PlayerChar->PlayerCamera->GetForwardVector());
	}

	return -1.0; // -1.0 will be opposite of the arrow, never a secret there
}

void AInspectableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!ScaleLerp.IsComplete())
	{
		ScaleLerp.Tick(DeltaSeconds);
		InspectRoot->SetRelativeScale3D(FMath::Lerp(FVector::OneVector, InspectScale, ScaleLerp.Current));
	}

	if (PlayerChar)
	{
		InspectRoot->SetWorldRotation(PlayerChar->GetInspectRotation());
		if (Archive)
		{
			if (!bArchived)
			{
				bArchived = true;
				PlayerChar->Inventory->AddArchive(Archive, false);
			}
			else if (!bSecretKnown && GetSecretDotAngle() >= MinSecretDotAngle)
			{
				bSecretKnown = true;
				PlayerChar->Inventory->AddArchive(Archive, true);
			}
		}
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
	SecretAngle->SetVisibility(IsValid(Archive));
#endif
}
