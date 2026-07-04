// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePlayerCharacter.h"

AGamePlayerCharacter::AGamePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void AGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGamePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
