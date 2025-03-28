﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/Fields/InteractField.h"
#include "Components/BrushComponent.h"

AInteractField::AInteractField() : bSingleUse(true)
{
	WorldEventComponent = CreateDefaultSubobject<UWorldActionComponent>("WorldEvents");
	GetBrushComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Interaction.Label.IsEmptyOrWhitespace())
	{
		Interaction.Label = NSLOCTEXT("ToroUtilities", "Interact", "Interact");
	}
}

void AInteractField::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult)
{
	if (!bEnabled) return;
	OnInteracted.Broadcast(Player);
	OnInteractedEvent.Broadcast(Player);
	WorldEventComponent->RunEvents();
	if (bSingleUse) SetEnabled(false);
}
