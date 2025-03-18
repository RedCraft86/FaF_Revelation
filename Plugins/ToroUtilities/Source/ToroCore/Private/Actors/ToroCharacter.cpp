// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroCharacter.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(Character)

AToroCharacter::AToroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}
