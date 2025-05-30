// Copyright (C) RedCraft86. All Rights Reserved.

#include "InventoryComponent.h"

#define InvItems InvData.Items
#define InvArchives InvData.Archives
#define InvEquipment InvData.Equipment

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

uint8 UInventoryComponent::AddItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 Amount)
{
	if (Amount == 0) return 0;
	const UInventoryItemData* Item = InItem.LoadSynchronous();
	if (!Item || Item->ItemType == EInventoryItemType::Archive) return Amount;

	uint8 Overflow = Amount;
	if (Item->ItemType == EInventoryItemType::Generic)
	{
		if (InvItems.Contains(InItem))
		{
			const int32 Target = InvItems.FindRef(InItem) + Amount;
			Overflow = FMath::Min(0, Target - Item->StackSize);
			InvItems.Add(InItem, Target - Overflow);
		}
		else
		{
			Overflow = FMath::Min(0, Amount - Item->StackSize);
			InvItems.Add(InItem, Item->StackSize);
		}
	}
	else if (!InvItems.Contains(InItem)) // Also guaranteed to be equipment type
	{
		InvItems.Add(InItem, 1);
		Overflow--;
	}
	return Overflow;
}

uint8 UInventoryComponent::RemoveItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 Amount)
{
	if (Amount == 0) return 0;
	const UInventoryItemData* Item = InItem.LoadSynchronous();
	if (!Item || Item->ItemType == EInventoryItemType::Archive) return Amount;

	if (InvEquipment == InItem)
	{
		UnEquipItem();
	}

	uint8 Missing = Amount;
	if (Item->ItemType == EInventoryItemType::Generic)
	{
		if (InvItems.Contains(InItem))
		{
			const int32 Initial = InvItems.FindRef(InItem);
			if (const int32 Target = Initial - Amount; Target > 0)
			{
				InvItems.Add(InItem, Target);
			}
			else
			{
				Missing = FMath::Abs(Target);
				InvItems.Remove(InItem);
			}
		}
	}
	else if (!InvItems.Contains(InItem)) // Also guaranteed to be equipment type
	{
		InvItems.Remove(InItem);
		Missing--;
	}
	return Missing;
}

void UInventoryComponent::AddArchive(const TSoftObjectPtr<UInventoryItemData>& InArchive, const bool bShowHidden)
{
	if (const UInventoryItemData* Archive = InArchive.LoadSynchronous();
		!Archive || Archive->ItemType != EInventoryItemType::Archive)
		return;

	if (!InvArchives.Contains(InArchive) || (!InvArchives.FindRef(InArchive).bShowHidden && bShowHidden))
	{
		InvArchives.Add(InArchive, {InvArchives.Num(), bShowHidden});
	}
}

uint8 UInventoryComponent::GetItemAmount(const TSoftObjectPtr<UInventoryItemData>& InItem) const
{
	return InvArchives.Contains(InItem) ? 1 : InvItems.FindRef(InItem);
}

bool UInventoryComponent::HasItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 MinAmount) const
{
	return GetItemAmount(InItem) >= MinAmount;
}

void UInventoryComponent::UnEquipItem()
{
	InvEquipment.Reset();
	if (EquipActor)
	{
		EquipActor->OnUnquip();
		EquipActor->K2_DestroyActor();
		EquipActor = nullptr;
	}
}

void UInventoryComponent::EquipItem(const TSoftObjectPtr<UInventoryItemData>& InItem)
{
	if (!InItem.LoadSynchronous()) return;
	if (InvEquipment.LoadSynchronous() || EquipActor)
	{
		UnEquipItem();
	}

	if (const UInventoryItemData* Item = InItem.LoadSynchronous();
		Item && Item->ItemType == EInventoryItemType::Equipment && !Item->Equipment.IsNull())
	{
		if (AInventoryEquipment* NewActor = GetWorld()->SpawnActor<AInventoryEquipment>(Item->Equipment.LoadSynchronous()))
		{
			InvEquipment = InItem;
			EquipActor = NewActor;
			EquipActor->OnEquip();
			// TODO Attachment
		}
	}
}

TArray<TSoftObjectPtr<UInventoryItemData>> UInventoryComponent::GetSortedItems()
{
	TArray<TSoftObjectPtr<UInventoryItemData>> Items;
	for (auto It = InvItems.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull() || It.Value() == 0) It.RemoveCurrent();
		else Items.Add(It.Key());
	}

	Items.Sort([](const TSoftObjectPtr<UInventoryItemData>& A, const TSoftObjectPtr<UInventoryItemData>& B)
	{
		const UInventoryItemData *ItemA = A.LoadSynchronous(), *ItemB = B.LoadSynchronous();
		return ItemA->Priority != ItemB->Priority
			? ItemA->Priority < ItemB->Priority
			: ItemA->DisplayName.CompareTo(ItemB->DisplayName) < 0;
	});

	return Items;
}

TArray<TSoftObjectPtr<UInventoryItemData>> UInventoryComponent::GetSortedArchives()
{
	TArray<TSoftObjectPtr<UInventoryItemData>> Archives;
	for (auto It = InvArchives.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull()) It.RemoveCurrent();
		else Archives.Add(It.Key());
	}

	Archives.Sort([this](const TSoftObjectPtr<UInventoryItemData>& A, const TSoftObjectPtr<UInventoryItemData>& B)
	{
		return InvArchives[A].ArchiveIdx > InvArchives[B].ArchiveIdx;
	});

	return Archives;
}

void UInventoryComponent::LoadData(const FInventoryData& InData)
{
	InvData.Items = InData.Items;
	InvData.Archives = InData.Archives;
	EquipItem(InData.Equipment.LoadSynchronous());
}
