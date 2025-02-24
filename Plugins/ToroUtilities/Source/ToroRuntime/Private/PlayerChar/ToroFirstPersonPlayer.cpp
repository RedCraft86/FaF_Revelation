// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerChar/ToroFirstPersonPlayer.h"

AToroFirstPersonPlayer::AToroFirstPersonPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
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
