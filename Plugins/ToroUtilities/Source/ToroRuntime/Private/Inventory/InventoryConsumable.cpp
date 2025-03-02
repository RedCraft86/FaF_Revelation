// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryConsumable.h"

void UInventoryConsumable::MarkFinished()
{
	MarkAsGarbage();
}

void UInventoryConsumable::UseConsumable(UInventoryComponent* Inventory)
{
	ConsumeItem(Inventory);
	if (bAutoMarkGarbage) MarkFinished();
}

UWorld* UInventoryConsumable::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}
