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
			return InSlot.Metadata.HasAnyMetadata(Metadata, !bCompareValues);
		case EInventoryMetaFilterMode::MatchAll:
			return InSlot.Metadata.HasAllMetadata(Metadata, !bCompareValues);
		}
	}

	return false;
}

bool FInventoryMetaFilter::Filter(const UInventoryItemData* InItem, const FInventorySlotData& InSlot) const
{
	if (InSlot.IsValidData() && InItem && InSlot.Item.LoadSynchronous() == InItem)
	{
		return Filter(InSlot);
	}

	return false;
}

FInventorySlotData::FInventorySlotData(const TObjectPtr<UInventoryItemData>& InItem, const uint8 InAmount, const FInventoryMetadata& InMetadata)
	: Item(InItem), Amount(InAmount)
{
	Metadata = InItem->DefaultMetadata;
	Metadata.Append(InMetadata);
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
}

TArray<FGuid> UInventoryComponent::GetSortedSlots() const
{
	TArray<TPair<FGuid, FInventorySlotData>> Slots;
	Slots.Reserve(ItemSlots.Num());

	for (const TPair<FGuid, FInventorySlotData>& Slot : ItemSlots)
	{
		if (Slot.Value.IsValidData())
		{
			Slots.Emplace(Slot.Key, Slot.Value);
		}
	}

	Slots.Sort([](const TPair<FGuid, FInventorySlotData>& A, const TPair<FGuid, FInventorySlotData>& B)
	{
		const UInventoryItemData *ItemA = A.Value.Item.LoadSynchronous(), *ItemB = B.Value.Item.LoadSynchronous();
		if (ItemA->Priority != ItemB->Priority)
		{
			return ItemA->Priority < ItemB->Priority;
		}

		return ItemA->DisplayName.CompareTo(ItemB->DisplayName) < 0;
	});

	TArray<FGuid> Out;
	Out.Reserve(Slots.Num());
	for (const TPair<FGuid, FInventorySlotData>& Slot : Slots)
	{
		Out.Emplace(Slot.Key);
	}

	return Out;
}

FGuid UInventoryComponent::FindSlot(const UInventoryItemData* Item, const FInventoryMetaFilter& Filter)
{
	for (const TPair<FGuid, FInventorySlotData>& Slot : ItemSlots)
	{
		if (Filter.Filter(Item, Slot.Value)) return Slot.Key;
	}

	return {};
}

TSet<FGuid> UInventoryComponent::FindSlots(const UInventoryItemData* Item, const FInventoryMetaFilter& Filter)
{
	TSet<FGuid> Matches;
	for (const TPair<FGuid, FInventorySlotData>& Slot : ItemSlots)
	{
		if (Filter.Filter(Item, Slot.Value)) Matches.Add(Slot.Key);
	}

	return {};
}

void UInventoryComponent::AppendSlotMetadata(const FGuid& InSlot, const FInventoryMetadata& InMetadata)
{
	if (InSlot.IsValid() && ItemSlots.Contains(InSlot))
	{
		ItemSlots[InSlot].Metadata.Append(InMetadata);
	}
}

void UInventoryComponent::AddSlotMetadata(const FGuid& InSlot, const FGameplayTag InKey, const FString InValue)
{
	if (InSlot.IsValid() && ItemSlots.Contains(InSlot))
	{
		ItemSlots[InSlot].Metadata.Add(InKey, InValue);
	}
}

void UInventoryComponent::RemoveSlotMetadata(const FGuid& InSlot, const FGameplayTag InKey)
{
	if (InSlot.IsValid() && ItemSlots.Contains(InSlot))
	{
		ItemSlots[InSlot].Metadata.Remove(InKey);
	}
}

bool UInventoryComponent::SlotHasMetadata(const FGuid& InSlot, const FGameplayTag InKey, const FString InValue)
{
	if (InSlot.IsValid() && ItemSlots.Contains(InSlot) && !InKey.IsValid())
	{
		return ItemSlots[InSlot].Metadata.HasMetadata({InKey, InValue},
			!InValue.Equals(TEXT("*FilterAny")));
	}

	return false;
}

uint8 UInventoryComponent::AddItemToSlot(const FGuid& InSlot, const uint8 Amount, const bool bSilent)
{
	if (InSlot.IsValid() && ItemSlots.Contains(InSlot) && Amount > 0)
	{
		uint8 Overflow = 0;
		FInventorySlotData& SlotRef = ItemSlots[InSlot];

		SlotRef.Amount += Amount;
		const UInventoryItemData* Item = SlotRef.Item.LoadSynchronous();
		const uint8 StackSizeLimit = Item->GetStackLimit();
		if (SlotRef.Amount > StackSizeLimit)
		{
			Overflow = SlotRef.Amount - StackSizeLimit;
			SlotRef.Amount = StackSizeLimit;
		}

		ValidateInventory();
		if (!bSilent) ON_ITEM_ADDED(Item, Amount - Overflow);
		return Overflow;
	}

	return Amount;
}

void UInventoryComponent::AddInventoryItem(uint8& Overflow, TSet<FGuid>& Slots, const UInventoryItemData* Item, const uint8 Amount, const FInventoryMetadata& Metadata, const bool bSilent)
{
	// TODO: implement AddInventoryItem
}

uint8 UInventoryComponent::AddItem(const UInventoryItemData* Item, const uint8 Amount, const FInventoryMetadata& Metadata, const bool bSilent)
{
	uint8 Overflow;
	TSet<FGuid> Slots;
	AddInventoryItem(Overflow, Slots, Item, Amount, Metadata, bSilent);
	return Overflow;
}

uint8 UInventoryComponent::RemoveItemFromSlot(const FGuid& InSlot, const uint8 Amount, const bool bSilent)
{
	if (ItemSlots.Contains(InSlot) && Amount > 0)
	{
		uint8 Missing = 0;
		bool bChanged = false;
		FInventorySlotData& SlotRef = ItemSlots[InSlot];

		SlotRef.Amount -= Amount;
		const UInventoryItemData* Item = SlotRef.Item.LoadSynchronous();
		if (SlotRef.Amount <= 0)
		{
			Missing = FMath::Abs(SlotRef.Amount);
			ItemSlots.Remove(InSlot);
			bChanged = true;
		}

		ValidateInventory(bChanged);
		if (!bSilent) ON_ITEM_REMOVED(Item, Amount - Missing);
		return Missing;
	}

	return Amount;
}

void UInventoryComponent::RemoveInventoryItem(uint8& Missing, const UInventoryItemData* Item, const uint8 Amount, const FInventoryMetaFilter& Filter, const bool bSilent)
{
	// TODO: implement RemoveInventoryItem
}

uint8 UInventoryComponent::RemoveItem(const UInventoryItemData* Item, const uint8 Amount, const FInventoryMetaFilter& Filter, const bool bSilent)
{
	uint8 Missing;
	RemoveInventoryItem(Missing, Item, Amount, Filter, bSilent);
	return Missing;
}

void UInventoryComponent::ValidateInventory(const bool bForceUpdate)
{
	bool bChanged = bForceUpdate;
	for (auto It = ItemSlots.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid() || !It.Value().IsValidData())
		{
			bChanged = true;
			It.RemoveCurrent();
		}
	}
	if (bChanged) ON_UPDATE()
}
