// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/TeleportTargetActor.h"
#include "Components/CapsuleComponent.h"

ATeleportTargetActor::ATeleportTargetActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATeleportTargetActor::TeleportActor(AActor* TargetActor)
{
	if (AToroPlayerBase* AsPlayer = Cast<AToroPlayerBase>(TargetActor))
	{
		PlayerRef = TObjectPtr<AToroPlayerBase>(AsPlayer);
		TeleportPlayer();
	}
	else
	{
		TargetActor->SetActorLocation(GetActorLocation());
		TargetActor->SetActorRotation(GetActorRotation());
	}
}

void ATeleportTargetActor::TeleportPlayer() const
{
	if (AToroPlayerBase* Player = PlayerRef.LoadSynchronous())
	{
		Player->SetActorLocation(GetActorLocation());
		Player->Controller->SetControlRotation(GetActorRotation());
	}
}

void ATeleportTargetActor::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerRef.IsNull()) PlayerRef = AToroPlayerBase::Get(this);
	if (const AToroPlayerBase* Player = PlayerRef.LoadSynchronous())
	{
		FHitResult Result;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(Player);
		if (GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation(),
			GetActorLocation() - FVector(0.0f, 0.0f, 100.0f), ECC_Visibility, Params))
		{
			SetActorLocation(Result.Location + FVector(0.0f, 0.0f,
				Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		}
	}
}

#if WITH_EDITOR
void ATeleportTargetActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector::OneVector);
	PlayerRef = AToroPlayerBase::Get(this);
	if (FRotator Rotation = GetActorRotation(); !FMath::IsNearlyZero(Rotation.Roll))
	{
		Rotation.Roll = 0.0f;
		SetActorRotation(Rotation);
	}
}
#endif
