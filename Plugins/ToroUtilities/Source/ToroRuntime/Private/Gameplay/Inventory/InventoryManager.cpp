// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/InventoryManager.h"
#include "UserInterface/ToroWidgetManager.h"
#include "SaveSystem/ToroGameSave.h"

namespace InventoryTags
{
	DEFINE_GAMEPLAY_TAG(Inventory)
}

void UInventoryManager::AddItem(UInventoryAsset* InItem, const uint8 Amount)
{
	uint8& Count = Items.FindOrAdd(InItem);
	Count += Amount;
	UpdateUI();
}

bool UInventoryManager::RemoveItem(UInventoryAsset* InItem, const uint8 Amount)
{
	uint8* Count = Items.Find(InItem);
	if (Count && *Count > Amount)
	{
		*Count = FMath::Max(*Count - Amount, 0);
		if (*Count == 0) Items.Remove(InItem);
		UpdateUI();
		return true;
	}
	return false;
}

void UInventoryManager::AddArchive(UInventoryAsset* InItem)
{
	if (!Archives.Contains(InItem))
	{
		Archives.Add(InItem);
	}
}

void UInventoryManager::UnEquipItem()
{
	if (Equipment.Actor)
	{
		Equipment.Actor->UnequipItem();
		Equipment = FInventoryEquipment();
		UpdateUI();
	}
}

void UInventoryManager::EquipItem(UInventoryAsset* InItem)
{
	if (!InItem || !InItem->IsValidEquipment() || Equipment.Item == InItem) return;
	
	if (Equipment.Actor)
	{
		UnEquipItem();
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewItem = InItem]()
	{
		Equipment.Actor = GetWorld()->SpawnActor<AEquipmentActor>(
			NewItem->Equipment.LoadSynchronous());
		if (Equipment.Actor)
		{
			Equipment.Item = NewItem;
			Equipment.Actor->EquipItem();
			UpdateUI();
		}
	});
}

void UInventoryManager::SetEquipmentUsage(const bool bUsing) const
{
	if (IsValid(Equipment.Actor))
	{
		Equipment.Actor->SetUseState(bUsing);
	}
}

void UInventoryManager::OpenInventory()
{
	if (UInventoryWidget* Widget = GetInventoryWidget())
	{
		Widget->PushWidget();
	}
}

void UInventoryManager::CloseInventory()
{
	if (UInventoryWidget* Widget = GetInventoryWidget())
	{
		Widget->PopWidget();
	}
}

bool UInventoryManager::IsInventoryOpen()
{
	UInventoryWidget* Widget = GetInventoryWidget();
	return Widget && Widget->IsPushed();
}

void UInventoryManager::PullFromSave(const FGameplayTag& Profile)
{
	if (!InventoryTags::IsValidTag(Profile)) return;
	if (const UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		Archives = Save->Archives;
		Items = Save->Items.FindRef(Profile).ItemMap;
		EquipItem(Save->Equipment.LoadSynchronous());
	}
}

void UInventoryManager::PushToSave(const FGameplayTag& Profile) const
{
	if (!InventoryTags::IsValidTag(Profile)) return;
	if (UToroGameSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGameSave>() : nullptr)
	{
		Save->Archives = Archives;
		Save->Items.Add(Profile, FInventoryItems(Items));
		Save->Equipment = Equipment.Item;
	}
}

void UInventoryManager::EnsureInventory(
	const TArray<TSoftObjectPtr<UInventoryAsset>>& InArchives,
	const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems)
{
	for (const TSoftObjectPtr<UInventoryAsset>& Archive : InArchives)
	{
		if (!Archives.Contains(Archive)) Archives.Add(Archive);
	}
	for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : InItems)
	{
		uint8& Amount = Items.FindOrAdd(Item.Key);
		Amount = FMath::Max(Amount, Item.Value);
	}
}

UInventoryWidget* UInventoryManager::GetInventoryWidget()
{
	if (!InventoryWidget)
	{
		InventoryWidget = AToroWidgetManager::GetWidget<UInventoryWidget>(this);
	}
	return InventoryWidget;
}

void UInventoryManager::UpdateUI()
{
	if (UInventoryWidget* Widget = GetInventoryWidget())
	{
		Widget->UpdateWidget();
	}
}

void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = UToroSaveManager::Get(this);
}
