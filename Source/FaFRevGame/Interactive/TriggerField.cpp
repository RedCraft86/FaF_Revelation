// Copyright (C) RedCraft86. All Rights Reserved.

#include "TriggerField.h"
#include "Player/PlayerCharacter.h"

ATriggerField::ATriggerField(): bSingleTrigger(true), bTriggered(false)
{
	PrimaryActorTick.bCanEverTick = false;

	ActionManager = CreateDefaultSubobject<UWorldActionManager>("ActionManager");
	ActionManager->bAutoConstruction = false;
}

void ATriggerField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && (!bTriggered || !bSingleTrigger))
	{
		bTriggered = true;
		ActionManager->SetActions(Actions, false);
		ActionManager->RunActions();

		OnTriggered.Broadcast(this, Player);
		OnTriggeredBP.Broadcast(this, Player);
	}
}

#if WITH_EDITOR
void ATriggerField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionManager->SetActions(Actions, true);

	bDisplayShadedVolume = true;
	BrushColor = (FLinearColor(1.0f, 0.8f, 0.0f, 1.0f).Desaturate(0.3f) * 0.6f).ToFColor(true);
}
#endif