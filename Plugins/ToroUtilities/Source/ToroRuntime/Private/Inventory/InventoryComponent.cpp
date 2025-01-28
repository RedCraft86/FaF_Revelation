// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"

FInventorySlotData::FInventorySlotData(const TObjectPtr<UInventoryItemData>& InItem, const uint8 InAmount, const FInventoryMetadata& InMetadata)
	: Item(InItem), Amount(InAmount), Metadata(InMetadata)
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
