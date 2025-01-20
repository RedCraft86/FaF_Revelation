// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/Fields/TriggerField.h"
#include "Characters/ToroPlayerBase.h"
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
	if (AToroPlayerBase* Player = Cast<AToroPlayerBase>(OtherActor))
	{
		OnTriggered.Broadcast(Player);
		OnTriggeredEvent.Broadcast(Player);
		WorldEventComponent->RunEvents();
		if (bSingleUse) SetEnabled(false);
	}
}
