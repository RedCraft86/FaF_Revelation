// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroCharacterBase.h"
#include "ToroRuntime.h"

AToroCharacterBase::AToroCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterID = Tag_Player;
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
