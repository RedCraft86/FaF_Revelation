// Copyright (C) RedCraft86. All Rights Reserved.

#include "InteractableNPC.h"
#include "Components/CapsuleComponent.h"

AInteractableNPC::AInteractableNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	Marker = CreateDefaultSubobject<UInteractionMarker>("Marker");
	Marker->SetupAttachment(GetRootComponent());

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AInteractableNPC::SetMarkerState_Implementation(const bool bVisible)
{
	Marker->SetVisibility(bVisible);
}

bool AInteractableNPC::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
{
	Info = Interaction;
	return CharacterTags::IsValidTag(CharacterID) && Interaction.bEnabled;
}

void AInteractableNPC::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	OnInteracted.Broadcast(this, Player);
	OnInteractedBP.Broadcast(this, Player);
}

#if WITH_EDITOR
void AInteractableNPC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Interaction.Label.IsEmptyOrWhitespace())
	{
		Interaction.Label = FText::FromString(FName::NameToDisplayString(
			CharacterID.GetTagLeafName().ToString(), false));
	}
}
#endif