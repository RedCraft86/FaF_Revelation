﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponentBase.h"

bool FInventorySlotData::MatchesWith(const UInventoryItemDataBase* Item, const FInventoryItemFilter& FilterData) const
{
	if (ItemData.IsNull() || !IsValid(Item) || Item != ItemData.LoadSynchronous())
		return false;

	if (FilterData.MetaFilter == EInventoryMetaFilter::None)
		return true;

	bool bDataMatch = false;
	for (const TPair<FName, FString>& Pair : FilterData.Metadata)
	{
		bDataMatch = Metadata.Contains(Pair.Key) && Metadata.FindRef(Pair.Key) == Pair.Value;
		switch (FilterData.MetaFilter)
		{
		case EInventoryMetaFilter::None: return true;
		case EInventoryMetaFilter::MatchAny: if (bDataMatch) return true;
		case EInventoryMetaFilter::MatchAll: if (!bDataMatch) return false;
		}
	}

	return false;
}

UInventoryComponentBase::UInventoryComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

FGuid UInventoryComponentBase::FindSlot(const UInventoryItemDataBase* Item, const FInventoryItemFilter& Filter)
{
	for (const TPair<FGuid, FInventorySlotData>& Slot : ItemSlots)
	{
		if (Slot.Value.MatchesWith(Item, Filter)) return Slot.Key;
	}

	return {};
}

TSet<FGuid> UInventoryComponentBase::FindSlots(const UInventoryItemDataBase* Item, const FInventoryItemFilter& Filter)
{
	TSet<FGuid> SlotKeys;
	for (const TPair<FGuid, FInventorySlotData>& Slot : ItemSlots)
	{
		if (Slot.Value.MatchesWith(Item, Filter)) SlotKeys.Add(Slot.Key);
	}

	return SlotKeys;
}

void UInventoryComponentBase::AddSlotMetadata(const FGuid& InSlot, const FName MetaKey, const FString MetaValue)
{
	if (ItemSlots.Contains(InSlot))
	{
		ItemSlots[InSlot].Metadata.Add(MetaKey, MetaValue);
	}
}

void UInventoryComponentBase::AppendSlotMetadata(const FGuid& InSlot, const TMap<FName, FString>& Metadata)
{
	if (ItemSlots.Contains(InSlot))
	{
		ItemSlots[InSlot].Metadata.Append(Metadata);
	}
}

void UInventoryComponentBase::RemoveSlotMetadata(const FGuid& InSlot, const FName MetaKey)
{
	if (ItemSlots.Contains(InSlot))
	{
		ItemSlots[InSlot].Metadata.Remove(MetaKey);
	}
}

int32 UInventoryComponentBase::AddItemToSlot(const FGuid& SlotKey, const int32 Amount, const bool bSilent)
{
	if (!ItemSlots.Contains(SlotKey) || Amount <= 0) return Amount;
	
	int32 Overflow = 0;
	ItemSlots[SlotKey].Amount += Amount;
	const UInventoryItemDataBase* Item = ItemSlots[SlotKey].ItemData.LoadSynchronous();
	const int32 StackSizeLimit = Item->GetStackLimit();
	if (ItemSlots[SlotKey].Amount > StackSizeLimit)
	{
		Overflow = ItemSlots[SlotKey].Amount - StackSizeLimit;
		ItemSlots[SlotKey].Amount = StackSizeLimit;
	}

	if (!CleanInventory()) ON_UPDATE();
	if (!bSilent) ON_ITEM_ADDED(Item, Amount - Overflow);
	return Overflow;
}

int32 UInventoryComponentBase::RemoveItemFromSlot(const FGuid& SlotKey, const int32 Amount, const bool bSilent)
{
	if (!ItemSlots.Contains(SlotKey) || Amount <= 0) return Amount;
	
	int32 Missing = 0;
	ItemSlots[SlotKey].Amount -= Amount;
	const UInventoryItemDataBase* Item = ItemSlots[SlotKey].ItemData.LoadSynchronous();
	
	if (ItemSlots[SlotKey].Amount <= 0)
	{
		Missing = FMath::Abs(ItemSlots[SlotKey].Amount);
		ItemSlots.Remove(SlotKey);
	}

	if (!CleanInventory()) ON_UPDATE();
	if (!bSilent) ON_ITEM_REMOVED(Item, Amount - Missing);
	return Missing;
}

void UInventoryComponentBase::AddItemToInventory(int32& Overflow, TSet<FGuid>& Slots, const UInventoryItemDataBase* Item, const int32 Amount, const TMap<FName, FString>& Metadata, const bool bSilent)
{
	Slots = {};
	Overflow = 0;
	if (!IsValid(Item) || Amount <= 0) return;

	int32 Raw, Final;
	switch (Item->StackingMode)
	{
	case EInventoryItemStackType::Unique:
		{
			Raw = FindSlots(Item).Num() + Amount;
			Final = FMath::Min((int32)Item->MaxNumOfSlots, Raw);
			Overflow = FMath::Max(Raw - Final, 0);

			for (int i = 0; i < Final; i++)
			{
				const FGuid ItemGuid(FGuid::NewGuid());
				ItemSlots.Add(ItemGuid, FInventorySlotData(Item, 1, Metadata));
				Slots.Add(ItemGuid);
			}
		}
		break;
		
	default:
		{
			if (const FGuid Slot = FindSlot(Item); Slot.IsValid())
			{
				Raw = ItemSlots[Slot].Amount + Amount;
				Final = FMath::Min(Item->GetStackLimit(), Raw);
				Overflow = FMath::Max(Raw - Final, 0);
				
				ItemSlots[Slot].Amount = Final;
				ItemSlots[Slot].Metadata.Append(Metadata);
			}
			else
			{
				Raw = Amount;
				Final = FMath::Min(Item->GetStackLimit(), Raw);
				Overflow = FMath::Max(Raw - Final, 0);

				const FGuid ItemGuid(FGuid::NewGuid());
				ItemSlots.Add(ItemGuid, FInventorySlotData(Item, Final, Metadata));
				Slots.Add(ItemGuid);
			}
		}
		break;
	}

	if (Final <= 0) return;
	if (!CleanInventory()) ON_UPDATE();
	if (!bSilent) ON_ITEM_ADDED(Item, Final);
}

int32 UInventoryComponentBase::AddItem(const UInventoryItemDataBase* Item, const int32 Amount, const TMap<FName, FString>& Metadata, const bool bSilent)
{
	int32 Overflow;
	TSet<FGuid> Slots;
	AddItemToInventory(Overflow, Slots, Item, Amount, Metadata, bSilent);
	return Overflow;
}

void UInventoryComponentBase::RemoveItemFromInventory(int32& Missing, const UInventoryItemDataBase* Item, const int32 Amount, const FInventoryItemFilter& Filter, const bool bSilent)
{
	Missing = 0;
	if (!IsValid(Item) || Amount <= 0) return;

	int32 Raw, Final;
	switch (Item->StackingMode)
	{
	case EInventoryItemStackType::Unique:
		{
			const TSet<FGuid> Slots = FindSlots(Item, Filter);
			Raw = Slots.Num() - Amount;
			Missing = Raw < 0 ? FMath::Abs(Raw) : 0;
			Final = Amount - Missing;
			
			Raw = Final;
			for (const FGuid& Slot : Slots)
			{
				Raw--;
				ItemSlots.Remove(Slot);
				if (Raw <= 0) break;
			}
		}
		break;
		
	default:
		{
			if (const FGuid Slot = FindSlot(Item); Slot.IsValid())
			{
				Raw = ItemSlots[Slot].Amount - Amount;
				Missing = Raw < 0 ? FMath::Abs(Raw) : 0;
				Final = Amount - Missing;

				ItemSlots[Slot].Amount -= Final;
				if (ItemSlots[Slot].Amount <= 0)
				{
					ItemSlots.Remove(Slot);
				}
			}
			else
			{
				Missing = Amount;
				Final = 0;
			}
		}
		break;
	}

	if (Final <= 0) return;
	if (!CleanInventory()) ON_UPDATE();
	if (!bSilent) ON_ITEM_REMOVED(Item, Final);
}

int32 UInventoryComponentBase::RemoveItem(const UInventoryItemDataBase* Item, const int32 Amount, const FInventoryItemFilter& Filter, const bool bSilent)
{
	int32 Missing;
	RemoveItemFromInventory(Missing, Item, Amount, Filter, bSilent);
	return Missing;
}

bool UInventoryComponentBase::CleanInventory()
{
	TArray<FGuid> Slots;
	ItemSlots.GenerateKeyArray(Slots);

	bool bHasAnyChange = false;
	for (const FGuid& Slot : Slots)
	{
		if (!ItemSlots[Slot].IsValidSlot())
		{
			bHasAnyChange = true;
			ItemSlots.Remove(Slot);
		}
	}

	if (bHasAnyChange) ON_UPDATE();
	return bHasAnyChange;
}
