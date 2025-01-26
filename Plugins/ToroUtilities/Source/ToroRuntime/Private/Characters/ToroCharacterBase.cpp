// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroCharacterBase.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Character, "Character");
UE_DEFINE_GAMEPLAY_TAG(Tag_Friendly, "Character.Friendly");
UE_DEFINE_GAMEPLAY_TAG(Tag_Enemy, "Character.Enemy");

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
