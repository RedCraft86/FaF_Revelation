// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePlayerController.h"
#include "GamePlayerCharacter.h"

AGamePlayerController::AGamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePlayerController::SetCinematicMode(bool bInCinematicMode, bool bAffectsMovement, bool bAffectsTurning)
{
	Super::SetCinematicMode(bInCinematicMode, bAffectsMovement, bAffectsTurning);
	if (AGamePlayerCharacter* PlayerChar = AGamePlayerCharacter::Get<AGamePlayerCharacter>(this))
	{
		PlayerChar->OnCinematic(bInCinematicMode);
	}
}
