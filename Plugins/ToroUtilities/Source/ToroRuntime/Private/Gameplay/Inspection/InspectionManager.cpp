// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inspection/InspectionManager.h"
#include "Gameplay/Inventory/InventoryManager.h"

UInspectionManager::UInspectionManager(): TurnRate(1.0f, 0.5f), bSecretKnown(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInspectionManager::BeginInspection(AInspectableActor* InInspectable)
{
	if (!InInspectable)
	{
		EndInspection();
		return;
	}

	if (!Inspectable.IsValid()) // no hot swapping
	{
		Inspectable = InInspectable;
		SetRelativeRotation(FRotator(0.0f, -10.0f, 0.0f));
		if (Inventory)
		{
			bSecretKnown = Inventory->AddArchive(Inspectable->Archive).bSecretFound;
		}

		// TODO ui

		OnInspection.Broadcast(true);
	}
}

void UInspectionManager::EndInspection()
{
	if (Inspectable.IsValid())
	{
		SetRelativeRotation(FRotator::ZeroRotator);

		// TODO ui

		Inspectable->OnEndInspect();
		Inspectable = nullptr;
		bSecretKnown = false;

		OnInspection.Broadcast(true);
	}
}

void UInspectionManager::OnMouseXY(const FVector2D& InValue)
{
	if (Inspectable.IsValid())
	{
		AddLocalRotation(FRotator(InValue.Y * TurnRate.Y, -InValue.X * TurnRate.X, 0.0f));
		if (!bSecretKnown && Inspectable->SeenSecret())
		{
			if (Inventory)
			{
				Inventory->AddArchive(Inspectable->Archive, true);
			}

			// TODO ui
			
			bSecretKnown = true;
		}
	}
}

void UInspectionManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Inventory = UInventoryManager::Get(this);
	});
}
