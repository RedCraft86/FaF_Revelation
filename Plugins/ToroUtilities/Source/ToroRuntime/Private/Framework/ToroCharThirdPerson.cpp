// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroCharThirdPerson.h"

AToroCharThirdPerson::AToroCharThirdPerson()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroCharThirdPerson::BeginPlay()
{
	Super::BeginPlay();
}

void AToroCharThirdPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroCharThirdPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
