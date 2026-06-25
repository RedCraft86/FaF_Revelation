// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "TriggerField.h"
#include "Actors/ToroCharacter.h"
#include "Components/BrushComponent.h"

ATriggerField::ATriggerField()
{
	PrimaryActorTick.bCanEverTick = false;
	GetBrushComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATriggerField::BeginPlay()
{
	Super::BeginPlay();
	Actions.Initialize(this);
}

void ATriggerField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AToroCharacter* Interactor = Cast<AToroCharacter>(OtherActor);
	if (Interactor && IsEnabled(this))
	{
		OnTriggered.Broadcast(Interactor);
		OnTriggeredBP.Broadcast(Interactor);
		Actions.Execute(this);

		if (bSingleUse)
		{
			SetEnabled(this, false);
		}
	}
}
