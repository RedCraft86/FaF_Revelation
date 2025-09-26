// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventoryManager.h"
#include "SaveSystem/ToroGameSave.h"

namespace InventoryTags
{
	DEFINE_GAMEPLAY_TAG(Inventory)
}

void UInventoryManager::AddItem(UInventoryAsset* InItem, const uint8 Amount)
{
	Items.AddItem(InItem, Amount);
	UpdateUI();
}

bool UInventoryManager::TakeItem(UInventoryAsset* InItem, const uint8 Amount)
{
	if (Items.TakeItem(InItem, Amount))
	{
		UpdateUI();
		return true;
	}
	return false;
}

FInventoryArchiveSlot UInventoryManager::AddArchive(UInventoryAsset* InItem, const bool bSecretFound)
{
	if (const FInventoryArchiveSlot* Ptr = Archives.AddArchive(InItem, bSecretFound))
	{
		UpdateUI();
		return *Ptr;
	}
	return FInventoryArchiveSlot(); 
}

void UInventoryManager::UnEquipItem()
{
	if (Equipment)
	{
		Equipment->UnequipItem();
		Equipment = nullptr;

		Items.SetEquippedItem(nullptr);
		UpdateUI();
	}
}

void UInventoryManager::EquipItem(UInventoryAsset* InItem)
{
	if (!InItem || !InItem->IsValidEquipment() || Items.GetEquippedItem() == InItem) return;
	if (Equipment = GetWorld()->SpawnActor<AEquipmentActor>(InItem->Equipment.LoadSynchronous()); Equipment)
	{
		Items.SetEquippedItem(InItem);
		Equipment->EquipItem();
		UpdateUI();
	}
}

void UInventoryManager::SetEquipmentUsage(const bool bUsing) const
{
	if (Equipment)
	{
		Equipment->SetUseState(bUsing);
	}
}

void UInventoryManager::PullFromSave(const FGameplayTag& Profile)
{
	if (!InventoryTags::IsValidTag(Profile)) return;
	if (const UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		Archives = Save->Archives;
		Items = Save->Items.FindRef(Profile);

		// To apply equipment changes, manually clear equipment, then re-equip it
		if (Equipment) Equipment->UnequipItem();
		UInventoryAsset* Asset = Items.GetEquippedItem().LoadSynchronous();
		Items.SetEquippedItem(nullptr);
		EquipItem(Asset);
	}
}

void UInventoryManager::PushToSave(const FGameplayTag& Profile) const
{
	if (!InventoryTags::IsValidTag(Profile)) return;
	if (UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		Save->Archives = Archives;
		Save->Items.Add(Profile, Items);
	}
}

void UInventoryManager::UpdateUI()
{
	// TODO ui
}

void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = UToroSaveManager::Get(this);
}
