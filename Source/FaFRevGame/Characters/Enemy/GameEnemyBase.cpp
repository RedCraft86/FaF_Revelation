// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameEnemyBase.h"

AGameEnemyBase::AGameEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGameEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
