// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InteractField.h"
#include "Components/BrushComponent.h"

AInteractField::AInteractField()
{
	PrimaryActorTick.bCanEverTick = false;
	GetBrushComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractField::BeginPlay()
{
	Super::BeginPlay();
	Actions.Initialize(this);
}

bool AInteractField::GetInteractionInfo_Implementation(FText& Label)
{
	Label = LabelText;
	return IsEnabled(this);
}

void AInteractField::BeginInteract_Implementation(AToroCharacter* Interactor)
{
	if (IsEnabled(this))
	{
		OnInteraction.Broadcast(Interactor);
		OnInteractionBP.Broadcast(Interactor);
		Actions.Execute(this);

		if (bSingleUse)
		{
			SetEnabled(this, false);
		}
	}
}
