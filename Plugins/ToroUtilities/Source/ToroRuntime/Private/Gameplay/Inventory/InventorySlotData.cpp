// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventorySlotData.h"

void FInventoryItemSlot::AddAmount(const uint8 InAmount)
{
	if (InAmount == 0) return;
	if (const UInventoryAsset* AssetPtr = Asset.LoadSynchronous())
	{
		Amount = FMath::Min(Amount + InAmount, (int32)AssetPtr->GetStackLimit());
	}
}

bool FInventoryItemSlot::TakeAmount(const uint8 InAmount)
{
	if (Amount >= InAmount)
	{
		Amount -= InAmount;
		return true;
	}
	return false;
}

FJsonObjectWrapper FInventoryItemSlot::GetJsonMetadata() const
{
	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObjectFromString(Metadata);
	return Wrapper;
}

void FInventoryItemSlot::SetJsonMetadata(const FJsonObjectWrapper& InJson)
{
	InJson.JsonObjectToString(Metadata);
}

void FInventoryItems::CleanupAndSort()
{
	Slots.RemoveAllSwap([](const FInventoryItemSlot& Element)
	{
		return !Element.IsValidSlot();
	});

	Slots.Sort([](const FInventoryItemSlot& A, const FInventoryItemSlot& B)
	{
		const UInventoryAsset *ItemA = A.GetAsset(), *ItemB = B.GetAsset();
		return ItemA->DisplayName.CompareTo(ItemB->DisplayName) < 0;
	});
}

void FInventoryItems::ForEachSlot(const TFunction<void(const FInventoryItemSlot&)>& Func)
{
	if (!Func) return;
	CleanupAndSort();
	for (const FInventoryItemSlot& Item : Slots)
	{
		Func(Item);
	}
}

const FInventoryItemSlot* FInventoryItems::AddItem(UInventoryAsset* Item, const uint8 Amount)
{
	if (!Item || Item->AssetType != EInvAssetType::Item || Amount == 0) 
	{
		return nullptr;
	}
		
	if (FInventoryItemSlot* Slot = FindItem(Item))
	{
		Slot->AddAmount(Amount);
		return Slot;
	}

	return &Slots.Emplace_GetRef(Item, Amount);
}

bool FInventoryItems::TakeItem(const UInventoryAsset* Item, const uint8 Amount)
{
	if (!Item || Item->AssetType != EInvAssetType::Item || Amount == 0) 
	{
		return false;
	}

	const int32 Idx = Slots.IndexOfByPredicate([Item](const FInventoryItemSlot& Element)
	{
		return Element.Asset == Item;
	});

	if (Slots.IsValidIndex(Idx) && Slots[Idx].TakeAmount(Amount))
	{
		if (Slots[Idx].Amount == 0)
		{
			Slots.RemoveAtSwap(Idx);
		}
		return true;
	}

	return false;
}

FInventoryItemSlot* FInventoryItems::FindItem(const UInventoryAsset* Item)
{
	return Slots.FindByPredicate([Item](const FInventoryItemSlot& Element)
	{
		return Element.Asset == Item;
	});
}

bool FInventoryArchiveSlot::MarkSecretFound()
{
	if (!bSecretFound)
	{
		bSecretFound = true;
		return true;
	}
	return false;
}

void FInventoryArchives::ForEachSlot(const TFunction<void(const FInventoryArchiveSlot&)>& Func)
{
	if (!Func) return;
	for (const FInventoryArchiveSlot& Item : Slots)
	{
		Func(Item);
	}
}

const FInventoryArchiveSlot* FInventoryArchives::AddArchive(UInventoryAsset* Archive, const bool bSecretFound)
{
	if (!Archive || Archive->AssetType != EInvAssetType::Archive) 
	{
		return nullptr;
	}

	const int32 Idx = Slots.IndexOfByPredicate([Archive](const FInventoryArchiveSlot& Element)
	{
		return Element.Asset == Archive;
	});
       
	if (Slots.IsValidIndex(Idx))
	{
		FInventoryArchiveSlot* Slot = &Slots[Idx];
		if (bSecretFound && Slot->MarkSecretFound())
		{
			MoveToLast(Idx);
			return &Slots.Last();
		}
		return Slot;
	}

	return &Slots.Emplace_GetRef(Archive, bSecretFound);
}

void FInventoryArchives::MoveToLast(const int32 IndexToMove)
{
	if (Slots.IsValidIndex(IndexToMove) && IndexToMove != Slots.Num() - 1)
	{
		FInventoryArchiveSlot Item = MoveTemp(Slots[IndexToMove]);
		Slots.RemoveAt(IndexToMove);
		Slots.Add(MoveTemp(Item));
	}
}

FInventoryArchiveSlot* FInventoryArchives::FindArchive(const UInventoryAsset* Archive)
{
	return Slots.FindByPredicate([Archive](const FInventoryArchiveSlot& Element)
	{
		return Element.Asset == Archive;
	});
}
