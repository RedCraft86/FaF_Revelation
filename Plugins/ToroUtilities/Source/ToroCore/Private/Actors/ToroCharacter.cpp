// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroCharacter.h"

AToroCharacter::AToroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}
