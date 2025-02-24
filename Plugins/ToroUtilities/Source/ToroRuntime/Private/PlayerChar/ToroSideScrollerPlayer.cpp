// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerChar/ToroSideScrollerPlayer.h"

AToroSideScrollerPlayer::AToroSideScrollerPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroSideScrollerPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AToroSideScrollerPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroSideScrollerPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
