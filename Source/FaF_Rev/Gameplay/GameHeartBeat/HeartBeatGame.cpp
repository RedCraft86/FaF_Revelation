// Copyright (C) RedCraft86. All Rights Reserved.

#include "HeartBeatGame.h"

UHeartBeatGame::UHeartBeatGame()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeartBeatGame::BeginPlay()
{
	Super::BeginPlay();
}

void UHeartBeatGame::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
