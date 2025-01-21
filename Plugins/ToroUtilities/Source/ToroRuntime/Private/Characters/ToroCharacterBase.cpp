// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroCharacterBase.h"

AToroCharacterBase::AToroCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AToroCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
