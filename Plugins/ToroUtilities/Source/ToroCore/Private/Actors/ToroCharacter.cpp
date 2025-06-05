// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroCharacter.h"

namespace CharacterTags
{
	DEFINE_GAMEPLAY_TAG(Character)
	DEFINE_GAMEPLAY_TAG_CHILD(Character, Player)
}

void AToroCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}
