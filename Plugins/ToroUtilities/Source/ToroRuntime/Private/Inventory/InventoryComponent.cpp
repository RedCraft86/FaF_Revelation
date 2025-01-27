// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemData.h"

TMap<FGameplayTag, FString> FInventorySlotData::GetMetadata() const
{
	TMap<FGameplayTag, FString> Out;
	if (const UInventoryItemData* ItemObj = GetItem())
	{
		Out = ItemObj->DefaultMetadata;
		Out.Append(Metadata);
	}

	return Out;
}

UInventoryItemData* FInventorySlotData::GetItem() const
{
	return Item.LoadSynchronous();
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
}
