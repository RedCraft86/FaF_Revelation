// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerChar/ToroThirdPersonPlayer.h"

AToroThirdPersonPlayer::AToroThirdPersonPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroThirdPersonPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AToroThirdPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroThirdPersonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
