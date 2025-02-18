// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"
#include "Framework/ToroGameMode.h"

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

UInventoryComponent* UInventoryComponent::Get(const UObject* WorldContext)
{
	if (const AToroGameMode* GameMode = AToroGameMode::Get(WorldContext))
	{
		return GameMode->Inventory;
	}
	
	return nullptr;
}

TArray<FGuid> UInventoryComponent::GetSortedSlots() const
{
	TArray<TPair<FGuid, FInventorySlotData>> Slots;
	Slots.Reserve(ItemSlots.Num());

	for (const TPair<FGuid, FInventorySlotData>& Slot : ItemSlots)
	{
		if (Slot.Key.IsValid() && Slot.Value.IsValidData())
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
		Out.Add(Slot.Key);
	}

	return Out;
}

TArray<FGuid> UInventoryComponent::GetFilteredSlots(const TArray<FGuid>& InSlots, const TSet<EInventoryItemType>& TypeFilter, const bool bExcludeTypes) const
{
	TArray<FGuid> Out;
	for (const FGuid& Slot : InSlots)
	{
		if (!ItemSlots.Contains(Slot)) continue;
		if (const FInventorySlotData& SlotRef = ItemSlots[Slot]; SlotRef.IsValidData())
		{
			const bool bContains = TypeFilter.Contains(SlotRef.Item.LoadSynchronous()->ItemType);
			if (bExcludeTypes ? !bContains : bContains) Out.Add(Slot);
		}
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

void UInventoryComponent::AddInventoryItem(uint8& Overflow, TSet<FGuid>& Slots, UInventoryItemData* Item, const uint8 Amount, const FInventoryMetadata& Metadata, const bool bSilent)
{
	Slots = {};
	Overflow = 0;
	if (IsValid(Item) && Amount > 0)
	{
		int32 Sum, Final;
		if (Item->StackingMode == EInventoryStackType::UniqueSlots)
		{
			uint8 Existing = FindSlots(Item).Num();
			Sum = Existing + Amount;
			Final = FMath::Min((int32)Item->StackingValue, Sum);
			
			for (int32 i = 0; i < Final - Existing; i++)
			{
				const FGuid ItemGuid(FGuid::NewGuid());
				FInventorySlotData NewSlot(Item, 1, Metadata);
				ItemSlots.Add(ItemGuid, NewSlot);
				Slots.Add(ItemGuid);
			}
		}
		else
		{
			if (const FGuid Slot = FindSlot(Item); Slot.IsValid())
			{
				FInventorySlotData& SlotRef = ItemSlots[Slot];
				Sum = SlotRef.Amount + Amount;
				Final = FMath::Min((int32)Item->GetStackLimit(), Sum);
				Overflow = FMath::Max(Sum - Final, 0);
				
				SlotRef.Amount = (uint8)Final;
				SlotRef.Metadata.Append(Metadata);
			}
			else
			{
				Sum = Amount;
				Final = FMath::Min((int32)Item->GetStackLimit(), Sum);
				Overflow = FMath::Max(Sum - Final, 0);
				
				const FGuid ItemGuid(FGuid::NewGuid());
				FInventorySlotData NewSlot(Item, (uint8)Final, Metadata);
				ItemSlots.Add(ItemGuid, NewSlot);
				Slots.Add(ItemGuid);
			}
		}

		if (Final > 0)
		{
			ValidateInventory();
			if (!bSilent) ON_ITEM_ADDED(Item, Final);
		}
	}
}

uint8 UInventoryComponent::AddItem(UInventoryItemData* Item, const uint8 Amount, const FInventoryMetadata& Metadata, const bool bSilent)
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

void UInventoryComponent::RemoveInventoryItem(uint8& Missing, UInventoryItemData* Item, const uint8 Amount, const FInventoryMetaFilter& Filter, const bool bSilent)
{
	Missing = 0;
	if (IsValid(Item) && Amount > 0)
	{
		int32 Sum, Final;
		if (Item->StackingMode == EInventoryStackType::UniqueSlots)
		{
			const TSet<FGuid> Slots = FindSlots(Item, Filter);
			Sum = Slots.Num() - Amount;
			Missing = Sum < 0 ? FMath::Abs(Sum) : 0;
			Final = Amount - Missing;
			Sum = Final;
			
			for (const FGuid& Slot : Slots)
			{
				Sum--;
				ItemSlots.Remove(Slot);
				if (Sum <= 0) break;
			}
		}
		else
		{
			if (const FGuid Slot = FindSlot(Item); Slot.IsValid())
			{
				FInventorySlotData& SlotRef = ItemSlots[Slot];
				Sum = SlotRef.Amount - Amount;
				Missing = Sum < 0 ? FMath::Abs(Sum) : 0;
				Final = Amount - Missing;

				SlotRef.Amount = FMath::Max(SlotRef.Amount - Final, 0);
				if (!SlotRef.IsValidData())
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

		if (Final > 0)
		{
			ValidateInventory();
			if (!bSilent) ON_ITEM_REMOVED(Item, Final);
		}
	}
}

uint8 UInventoryComponent::RemoveItem(UInventoryItemData* Item, const uint8 Amount, const FInventoryMetaFilter& Filter, const bool bSilent)
{
	uint8 Missing;
	RemoveInventoryItem(Missing, Item, Amount, Filter, bSilent);
	return Missing;
}

void UInventoryComponent::EnsureItems(const TArray<FInventorySlotData>& InItems)
{
	for (const FInventorySlotData& InSlot : InItems)
	{
		UInventoryItemData* ItemData = InSlot.Item.LoadSynchronous(); if (!ItemData) continue;
		if (const FGuid SlotID = FindSlot(ItemData); SlotID.IsValid() && ItemSlots.Contains(SlotID))
		{
			FInventorySlotData& SlotData = ItemSlots[SlotID];
			SlotData.Amount = FMath::Min(FMath::Max(SlotData.Amount, InSlot.Amount), ItemData->GetStackLimit());
			SlotData.Metadata.Append(InSlot.Metadata);
			SlotData.Metadata.Validate();
		}
		else
		{
			AddItem(ItemData, FMath::Min(InSlot.Amount, ItemData->GetStackLimit()), InSlot.Metadata, true);
		}
	}
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
