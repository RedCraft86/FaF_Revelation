// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/InteractionField.h"
#include "Components/BrushComponent.h"

AInteractionField::AInteractionField()
{
	ActionComponent = CreateDefaultSubobject<UWorldActionComponent>("ActionComponent");
	ActionComponent->bAutoConstruction = false;

	if (UBrushComponent* BrushComp = GetBrushComponent())
	{
		BrushComp->SetCollisionObjectType(ECC_WorldDynamic);
		BrushComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		BrushComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		BrushComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

FInteractionInfo AInteractionField::GetInteractInfo_Implementation(const FHitResult& HitResult)
{
	return IsEnabled() ? Interaction : FInteractionInfo::GetEmpty();
}

void AInteractionField::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult)
{
	if (!IsEnabled()) return;
	OnInteracted.Broadcast(Player);
	OnInteractedEvent.Broadcast(Player);
	ActionComponent->SetActions(Actions);
	ActionComponent->RunActions();
	if (bSingleUse) SetEnabled(false);
}

void AInteractionField::OnConstruction(const FTransform& Transform)
{
	ActionComponent->SetActions(Actions, true);
}
