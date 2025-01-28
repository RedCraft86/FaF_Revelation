// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"

FInventorySlotData::FInventorySlotData(const UInventoryItemData* Data, const uint8 Amount, const FInventoryMetadata& InMetadata)
	: Item(Data), Amount(Amount), Metadata(InMetadata)
{
	Metadata.Validate();
}

FInventoryMetadata FInventorySlotData::GetMetadata() const
{
	FInventoryMetadata Out;
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
