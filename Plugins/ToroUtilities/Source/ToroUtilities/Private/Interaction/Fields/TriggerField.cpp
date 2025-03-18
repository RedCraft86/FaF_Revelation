// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/Fields/TriggerField.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Components/BrushComponent.h"

ATriggerField::ATriggerField() : bSingleUse(true)
{
	WorldEventComponent = CreateDefaultSubobject<UWorldActionComponent>("WorldEvents");
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATriggerField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!bEnabled) return;
	if (AToroPlayerCharacter* Player = Cast<AToroPlayerCharacter>(OtherActor))
	{
		OnTriggered.Broadcast(Player);
		OnTriggeredEvent.Broadcast(Player);
		WorldEventComponent->RunEvents();
		if (bSingleUse) SetEnabled(false);
	}
}
