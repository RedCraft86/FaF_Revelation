// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroCharacter.h"

DEFINE_GAMEPLAY_TAG_BASE(Character)

AToroCharacter::AToroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

FVector AToroCharacter::GetLookTarget() const
{
	return GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
}

void AToroCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}
