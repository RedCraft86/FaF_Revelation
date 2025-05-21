// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/TeleportPoint.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroCharacter.h"
#include "Kismet/GameplayStatics.h"

ATeleportPoint::ATeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATeleportPoint::TeleportActor(AActor* TargetActor) const
{
	if (!TargetActor) return;
	if (AToroCharacter* AsCharacter = Cast<AToroCharacter>(TargetActor))
	{
		AsCharacter->Teleport(GetActorLocation(), GetActorRotation());
	}
	else
	{
		TargetActor->SetActorLocation(GetActorLocation());
		TargetActor->SetActorRotation(GetActorRotation());
	}
}

void ATeleportPoint::TeleportPlayer() const
{
	TeleportActor(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void ATeleportPoint::BeginPlay()
{
	Super::BeginPlay();
	if (const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(Player);
		if (FHitResult Result; GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation(),
			GetActorLocation() - FVector(0.0f, 0.0f, 100.0f), ECC_Visibility, Params))
		{
			SetActorLocation(Result.Location + FVector(0.0f, 0.0f,
				Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		}
	}
}

void ATeleportPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorScale3D(FVector::OneVector);
	if (FRotator Rotation = GetActorRotation(); !FMath::IsNearlyZero(Rotation.Roll))
	{
		Rotation.Roll = 0.0f;
		SetActorRotation(Rotation);
	}
}
