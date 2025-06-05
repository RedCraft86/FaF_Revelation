// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/TeleportPoint.h"
#include "Actors/ToroCharacter.h"
#include "Kismet/GameplayStatics.h"
#if WITH_EDITORONLY_DATA
#include "Components/ArrowComponent.h"
#endif

ATeleportPoint::ATeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#if WITH_EDITORONLY_DATA
	GetArrowComponent()->SetRelativeScale3D(FVector(5.0f));
#endif
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

void ATeleportPoint::TeleportPlayer(const int32 PlayerIndex) const
{
	TeleportActor(UGameplayStatics::GetPlayerCharacter(this, PlayerIndex));
}

#if WITH_EDITOR
void ATeleportPoint::OffsetFromFloor()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0, 0, 10000.0f);

	FHitResult Hit;
	const FCollisionQueryParams Params(NAME_None, false, this);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params);
	DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Green, false, 2.0f);
	DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 2.0f);
	if (bHit)
	{
		const float DistanceToFloor = Start.Z - Hit.ImpactPoint.Z;
		if (!FMath::IsNearlyEqual(DistanceToFloor, DesiredOffset))
		{
			const float NeededOffset = DesiredOffset - DistanceToFloor;
			SetActorLocation(Start + FVector(0, 0, NeededOffset));
		}
	}
}

void ATeleportPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!bDidOffset)
	{
		bDidOffset = true;
		OffsetFromFloor();
	}

	SetActorScale3D(FVector::OneVector);
	if (FRotator Rotation = GetActorRotation(); !FMath::IsNearlyZero(Rotation.Roll))
	{
		Rotation.Roll = 0.0f;
		SetActorRotation(Rotation);
	}
}
#endif
