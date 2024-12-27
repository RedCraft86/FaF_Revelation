// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroCharFirstPerson.h"

AToroCharFirstPerson::AToroCharFirstPerson()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroCharFirstPerson::BeginPlay()
{
	Super::BeginPlay();
}

void AToroCharFirstPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroCharFirstPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
