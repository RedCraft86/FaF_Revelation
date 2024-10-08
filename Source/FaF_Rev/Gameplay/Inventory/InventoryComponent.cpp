﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "InventoryComponent.h"
#include "Menus/InventoryWidget.h"
#include "FRPlayerController.h"
#include "ConsumableActor.h"
#include "FRGameMode.h"
#include "FRPlayer.h"

UInventoryComponent::UInventoryComponent() : PlayerChar(nullptr), InventoryPreview(nullptr), InventoryWidget(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInventoryComponent::OpenUI()
{
	if (!InventoryWidget || bIsInInventory) return;
	PlayerChar->AddLockFlag(Player::LockFlags::Inventory);
	PlayerChar->GetGameMode()->SetGameInputMode(EGameInputMode::GameAndUI, true,
		EMouseLockMode::LockAlways, false, InventoryWidget);

	CleanInventory();
	InventoryWidget->AddWidget();
	if (InventoryPreview)
	{
		InventoryPreview->Initialize();
		InventoryPreview->SetItem({});
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		PlayerChar->GetPlayerController()->SetPause(true);
	});
	bIsInInventory = true;
}

void UInventoryComponent::CloseUI()
{
	if (!InventoryWidget || !bIsInInventory) return;
	PlayerChar->ClearLockFlag(Player::LockFlags::Inventory);
	PlayerChar->GetGameMode()->SetGameInputMode(EGameInputMode::GameOnly);
	PlayerChar->GetPlayerController()->SetPause(false);
	
	if (InventoryPreview)
	{
		InventoryPreview->Deinitialize();
	}
	InventoryWidget->RemoveWidget([this]()
	{
		bIsInInventory = false;
	});
}

void UInventoryComponent::EquipmentUse() const
{
	if (EquipmentData.Equipment)
	{
		EquipmentData.Equipment->OnUse();
	}
}

void UInventoryComponent::EquipmentUseAlt(const bool bPressed) const
{
	if (!EquipmentData.Equipment) return;
	if (bPressed)
	{
		EquipmentData.Equipment->OnStartAltUse();
	}
	else
	{
		EquipmentData.Equipment->OnEndAltUse();
	}
}

void UInventoryComponent::SetInventoryPreview(AInventoryPreview* InActor)
{
	InventoryPreview = InActor;
	if(InventoryPreview) InventoryPreview->Inventory = this;
}

TArray<FGuid> UInventoryComponent::GetSortedSlots(const EInventoryItemType TypeFilter)
{
	CleanInventory();
	TArray<FGuid> Result;
	ItemSlots.GenerateKeyArray(Result);
	Result.Sort([this](const FGuid& A, const FGuid& B)->bool
	{
		const UInventoryItemData* ItemA = ItemSlots[A].GetItemData<UInventoryItemData>();
		const UInventoryItemData* ItemB = ItemSlots[B].GetItemData<UInventoryItemData>();
		return ItemA->DisplayName.ToString() < ItemB->DisplayName.ToString() && ItemA->Priority <= ItemB->Priority;
	});

	if (TypeFilter != EInventoryItemType::Any)
	{
		Result.RemoveAll([this, TypeFilter](const FGuid& Elem)
		{
			return ItemSlots[Elem].GetItemData<UInventoryItemData>()->ItemType != TypeFilter;
		});
	}

	return Result;
}

bool UInventoryComponent::UseKeyItem(const UInventoryItemDataBase* InItem, const FString& KeyID)
{
	if (!InItem || KeyID.IsEmpty())
		return false;
	
	FInventoryItemFilter Filter;
	Filter.MetaFilter = EInventoryMetaFilter::MatchAll;
	Filter.Metadata.Add(NativeItemKeys::KeyID, KeyID);
	const FGuid Slot = FindSlot(InItem, Filter);
	if (!Slot.IsValid()) return false;
	
	if (SlotHasMetadata(Slot, NativeItemKeys::SingleKey))
		RemoveItemFromSlot(Slot, 1);
	
	return true;
}

void UInventoryComponent::ConsumeItem(const FGuid& ItemKey)
{
	const UInventoryItemData* ItemData = ItemSlots.FindRef(ItemKey).GetItemData<UInventoryItemData>();
	if (ItemData && ItemData->ItemType == EInventoryItemType::Consumable && ItemData->ConsumableClass)
	{
		AConsumableActor* Consumable = GetWorld()->SpawnActor<AConsumableActor>(ItemData->ConsumableClass);
		ItemSlots[ItemKey].Amount -= Consumable->UseItem() ? 1 : 0;
		if (ItemSlots[ItemKey].Amount <= 0)
		{
			ItemSlots.Remove(ItemKey);
		}
	}
}

void UInventoryComponent::UnequipItem()
{
	if (!EquipmentData.ItemID.IsValid()) return;
	if (EquipmentData.Equipment)
	{
		EquipmentData.Equipment->SetActorHiddenInGame(true);
		EquipmentData.Equipment->bEquipped = false;
		EquipmentData.Equipment->OnUnequip();
	}
	EquipmentData = {};
}

void UInventoryComponent::EquipItem(const FGuid& ItemKey)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(EquipTimer)) return;

	const UInventoryItemData* ItemData = ItemSlots.FindRef(ItemKey).GetItemData<UInventoryItemData>();
	if (!ItemData || ItemData->ItemType != EInventoryItemType::Equipment || !ItemData->EquipmentClass) return;

	float WaitTime = 0.01f;
	if (EquipmentData.ItemID.IsValid())
	{
		UnequipItem();
		WaitTime = 0.1f;
	}

	GetWorld()->GetTimerManager().SetTimer(EquipTimer, [this, ItemData, ItemKey]()
	{
		if (AEquipmentActor* Equipment = GetWorld()->SpawnActor<AEquipmentActor>(ItemData->ConsumableClass))
		{
			EquipmentData.ItemID = ItemKey;
			EquipmentData.Equipment = Equipment;
			Equipment->PlayerChar = PlayerChar;
			Equipment->Inventory = this;
			Equipment->ItemID = ItemKey;
			Equipment->bEquipped = true;
		}
		else
		{
			EquipmentData = {};
		}
	}, WaitTime, false);
}

void UInventoryComponent::EnsureItems(const TArray<FInventorySlotData>& InItems)
{
	for (const FInventorySlotData& InSlot : InItems)
	{
		const UInventoryItemDataBase* ItemData = InSlot.ItemData.LoadSynchronous(); if (!ItemData) continue;
		if (const FGuid SlotID = FindSlot(ItemData); SlotID.IsValid() && ItemSlots.Contains(SlotID))
		{
			FInventorySlotData& SlotData = ItemSlots[SlotID];
			SlotData.Amount = FMath::Min(FMath::Max(SlotData.Amount, InSlot.Amount), ItemData->GetStackLimit());
			SlotData.Metadata.Append(InSlot.Metadata);
			SlotData.ValidateMetadata();
		}
		else
		{
			AddItem(ItemData, FMath::Min(InSlot.Amount, ItemData->GetStackLimit()), InSlot.Metadata, true);
		}
	}
}

void UInventoryComponent::ImportSaveData(const FInventorySaveData& InData)
{
	CurrencyData = InData.CurrencyData;
	ItemSlots.Append(InData.ItemSlots);
	if (ItemSlots.Contains(InData.ActiveEquipment))
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, InData]()
		{
			EquipItem(InData.ActiveEquipment);
		});
	}

	ON_UPDATE();
}

FInventorySaveData UInventoryComponent::ExportSaveData()
{
	return {EquipmentData.ItemID, CurrencyData, ItemSlots};
}
