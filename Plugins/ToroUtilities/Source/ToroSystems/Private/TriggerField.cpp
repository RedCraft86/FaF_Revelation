// Copyright (C) RedCraft86. All Rights Reserved.

#include "TriggerField.h"
#include "Components/BrushComponent.h"

ATriggerField::ATriggerField()
{
	ActionComponent = CreateDefaultSubobject<UWorldActionComponent>("ActionComponent");
	ActionComponent->bAutoConstruction = false;

	GetBrushComponent()->SetCollisionProfileName("Trigger");
}

void ATriggerField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!IsEnabled()) return;
	if (AToroPlayerCharacter* Player = Cast<AToroPlayerCharacter>(OtherActor))
	{
		OnTriggered.Broadcast(Player);
		OnTriggeredEvent.Broadcast(Player);
		ActionComponent->SetActions(Actions);
		ActionComponent->RunActions();
		if (bSingleUse) SetEnabled(false);
	}
}

void ATriggerField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionComponent->SetActions(Actions, true);
}
