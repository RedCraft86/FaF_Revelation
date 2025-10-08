// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroCharacter.h"
#include "Components/CapsuleComponent.h"

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

bool AToroCharacter::GetFocusPoint_Implementation(FVector& Location) const
{
	const float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
	Location = GetActorLocation() + FVector{0.0f, 0.0f, Height};
	return true;
}

void AToroCharacter::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = GetPawnViewLocation();
	Forward = GetViewRotation().Vector();
	Angle = 60.0f;
}

void AToroCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!UniqueGuid.IsValid()) UniqueGuid = FGuid::NewGuid();
}
