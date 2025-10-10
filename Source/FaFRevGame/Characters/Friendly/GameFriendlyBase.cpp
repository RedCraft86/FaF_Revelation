// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameFriendlyBase.h"

AGameFriendlyBase::AGameFriendlyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameFriendlyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGameFriendlyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
