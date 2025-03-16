// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryConsumable.h"

void UInventoryConsumable::MarkFinished()
{
	MarkAsGarbage();
}

bool UInventoryConsumable::UseConsumable(UInventoryComponent* Inventory)
{
	const bool bSuccess = ConsumeItem(Inventory);
	if (bAutoMarkGarbage) MarkFinished();
	return bSuccess;
}

UWorld* UInventoryConsumable::GetWorld() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return GWorld;
#endif
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}
