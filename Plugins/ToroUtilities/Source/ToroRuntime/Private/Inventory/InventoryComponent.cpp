// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"

bool FInventoryMetaFilter::Filter(const FInventorySlotData& InSlot) const
{
	if (InSlot.IsValidData())
	{
		switch (FilterMode)
		{
		case EInventoryMetaFilterMode::Unfiltered: return true;
		case EInventoryMetaFilterMode::MatchAny:
			return InSlot.GetMetadata().HasAnyMetadata(Metadata, !bCompareValues);
		case EInventoryMetaFilterMode::MatchAll:
			return InSlot.GetMetadata().HasAllMetadata(Metadata, !bCompareValues);
		}
	}

	return false;
}

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
