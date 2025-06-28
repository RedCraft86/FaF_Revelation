// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerState.h"

#define InvItems InvData.Items
#define InvArchives InvData.Archives
#define InvEquipment InvData.Equipment

void FInventoryData::EnsureData(const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems,
	const TMap<TSoftObjectPtr<UInventoryAsset>, bool>& InArchives)
{
	for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : InItems)
	{
		if (Item.Key.IsNull()) continue;
		FInvItemSlot& Slot = Items.FindOrAdd(Item.Key);
		Slot.Amount = FMath::Max(Item.Value, Slot.Amount);
	}

	for (const TPair<TSoftObjectPtr<UInventoryAsset>, bool>& Archive : InArchives)
	{
		if (Archive.Key.IsNull()) continue;
		FInvArchiveSlot& Slot = Archives.FindOrAdd(Archive.Key);
		Slot.bKnowSecret = Slot.bKnowSecret || Archive.Value;
	}
}

UInventoryComponent::UInventoryComponent(): bInInventory(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UInventoryComponent* UInventoryComponent::GetInventoryComponent(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UInventoryComponent)
}

uint8 UInventoryComponent::AddItem(const TSoftObjectPtr<UInventoryAsset>& InItem, const uint8 Amount, const FString& Json)
{
	const UInventoryAsset* Item = InItem.LoadSynchronous();
	if (!Item || Amount == 0) return 0;
	if (Item->Type == EInvAssetType::Archive)
	{
		AddArchive(InItem, true);
		return 0;
	}

	uint8 Overflow = Amount;
	const uint8 StackLimit = Item->GetStackLimit();
	if (Item->ItemType == EInvItemType::Generic)
	{
		if (InvItems.Contains(InItem))
		{
			const int32 Target = InvItems.FindRef(InItem).Amount + (int32)Amount;
			Overflow = FMath::Min(0, Target - StackLimit);
			InvItems.Add(InItem).Amount = Target - Overflow;
		}
		else
		{
			Overflow = FMath::Min(0, Amount - StackLimit);
			InvItems.Add(InItem).Amount = StackLimit;
		}
	}
	else if (!InvItems.Contains(InItem)) // Also guaranteed to be key or equipment type
	{
		InvItems.Add(InItem).SetJson(Json);
		Overflow--;
	}
	return Overflow;
}

uint8 UInventoryComponent::RemoveItem(const TSoftObjectPtr<UInventoryAsset>& InItem, const uint8 Amount)
{
	const UInventoryAsset* Item = InItem.LoadSynchronous();
	if (!Item || Item->Type != EInvAssetType::Item || Amount == 0) return 0;
	if (InvEquipment == InItem)
	{
		UnEquipItem();
	}

	uint8 Missing = Amount;
	if (Item->ItemType == EInvItemType::Generic)
	{
		if (InvItems.Contains(InItem))
		{
			const int32 Initial = InvItems.FindRef(InItem).Amount;
			if (const int32 Target = Initial - Amount; Target > 0)
			{
				InvItems.Add(InItem).Amount = Target;
			}
			else
			{
				Missing = FMath::Abs(Target);
				InvItems.Remove(InItem);
			}
		}
	}
	else if (!InvItems.Contains(InItem)) // Also guaranteed to be key or equipment type
	{
		InvItems.Remove(InItem);
		Missing--;
	}
	return Missing;
}

bool UInventoryComponent::UseKeyItem(const TSoftObjectPtr<UInventoryAsset>& InItem)
{
	const UInventoryAsset* Item = InItem.LoadSynchronous();
	if (!Item || Item->Type != EInvAssetType::Item || Item->ItemType != EInvItemType::KeyItem) return false;
	const bool bHasItem = InvItems.Contains(InItem);
	if (bHasItem && !Item->bReusableKey)
	{
		RemoveItem(InItem, 1);
	}
	return bHasItem;
}

bool UInventoryComponent::GetItemSlot(const TSoftObjectPtr<UInventoryAsset>& InItem, FInvItemSlot& Slot) const
{
	const bool bHasItem = InItem.LoadSynchronous() && InvItems.Contains(InItem);
	if (bHasItem) Slot = InvItems[InItem];
	return bHasItem;
}

TArray<TSoftObjectPtr<UInventoryAsset>> UInventoryComponent::GetSortedItems()
{
	TArray<TSoftObjectPtr<UInventoryAsset>> Items;
	for (auto It = InvItems.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull() || It.Value().Amount == 0) It.RemoveCurrent();
		else Items.Add(It.Key());
	}

	Items.Sort([](const TSoftObjectPtr<UInventoryAsset>& A, const TSoftObjectPtr<UInventoryAsset>& B)
	{
		const UInventoryAsset *ItemA = A.LoadSynchronous(), *ItemB = B.LoadSynchronous();
		return ItemA->Priority != ItemB->Priority
			? ItemA->Priority < ItemB->Priority
			: ItemA->DisplayName.CompareTo(ItemB->DisplayName) < 0;
	});

	return Items;
}

void UInventoryComponent::AddArchive(const TSoftObjectPtr<UInventoryAsset>& InArchive, const bool bKnowSecret)
{
	if (const UInventoryAsset* Archive = InArchive.LoadSynchronous(); !Archive || !Archive->IsValidArchive()) return;
	if (!InvArchives.Contains(InArchive) || (bKnowSecret && !InvArchives[InArchive].bKnowSecret))
	{
		InvArchives.Add(InArchive, {bKnowSecret, InvArchives.Num()});
	}
}

bool UInventoryComponent::GetArchiveSlot(const TSoftObjectPtr<UInventoryAsset>& InArchive, FInvArchiveSlot& Slot) const
{
	const UInventoryAsset* Archive = InArchive.LoadSynchronous();
	const bool bHasItem = Archive && Archive->IsValidArchive() && InvArchives.Contains(InArchive);
	if (bHasItem) Slot = InvArchives[InArchive];
	return bHasItem;
}

TArray<TSoftObjectPtr<UInventoryAsset>> UInventoryComponent::GetSortedArchives()
{
	TArray<TSoftObjectPtr<UInventoryAsset>> Archives;
	for (auto It = InvArchives.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull()) It.RemoveCurrent();
		else Archives.Add(It.Key());
	}

	Archives.Sort([this](const TSoftObjectPtr<UInventoryAsset>& A, const TSoftObjectPtr<UInventoryAsset>& B)
	{
		return InvArchives[A].SortPriority > InvArchives[B].SortPriority;
	});

	return Archives;
}

void UInventoryComponent::UnEquipItem()
{
	InvEquipment.Reset();
	if (EquipActor)
	{
		EquipActor->OnUnequip();
		EquipActor->SetActorHiddenInGame(true);
		EquipActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		TWeakObjectPtr<AActor> WeakActor = EquipActor;
		GetWorld()->GetTimerManager().SetTimerForNextTick([WeakActor]()
		{
			if (WeakActor.IsValid())
			{
				WeakActor->K2_DestroyActor();
			}
		});
		EquipActor = nullptr;
	}
}

void UInventoryComponent::EquipItem(const TSoftObjectPtr<UInventoryAsset>& InItem)
{
	USceneComponent* AttachRoot = GetEquipmentRoot.IsBound() ? GetEquipmentRoot.Execute() : nullptr;
	if (InItem.IsNull() || !AttachRoot) return;

	if (InvEquipment.LoadSynchronous() || EquipActor)
	{
		UnEquipItem();
	}

	if (const UInventoryAsset* Item = InItem.LoadSynchronous(); Item && Item->IsValidEquipment())
	{
		if (AEquipmentActor* NewActor = GetWorld()->SpawnActor<AEquipmentActor>(Item->Equipment.LoadSynchronous()))
		{
			InvEquipment = InItem;
			EquipActor = NewActor;
			EquipActor->OnEquip();
			EquipActor->AttachToComponent(AttachRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void UInventoryComponent::SetEquipmentUsage(const bool bUsing) const
{
	if (EquipActor) EquipActor->SetUseState(bUsing);
}

void UInventoryComponent::OpenInventory()
{
	if (!bInInventory)
	{
		bInInventory = true;
		if (UInventoryWidget* InvWidget = GetWidget())
		{
			InvWidget->ActivateWidget();
		}
		if (AToroPlayerController* PC = AToroPlayerController::Get(this))
		{
			PC->AddPauseRequest(this);
			CachedInput = PC->GetInputModeData();
			PC->SetInputModeData({EGameInputMode::UI_Only, true});
		}
	}
}

void UInventoryComponent::CloseInventory()
{
	if (bInInventory)
	{
		bInInventory = false;
		if (UInventoryWidget* InvWidget = GetWidget())
		{
			InvWidget->DeactivateWidget();
		}
		if (AToroPlayerController* PC = AToroPlayerController::Get(this))
		{
			PC->RemovePauseRequest(this);
			PC->SetInputModeData(CachedInput);
		}
	}
}

void UInventoryComponent::LoadSaveData(const FInventoryData& InData)
{
	InvData.Items = InData.Items;
	InvData.Archives = InData.Archives;
	EquipItem(InData.Equipment.LoadSynchronous());
}

void UInventoryComponent::EnsureInventory(const TSoftObjectPtr<UInventoryAsset>& InEquipment,
	const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems,
	const TMap<TSoftObjectPtr<UInventoryAsset>, bool>& InArchives)
{
	InvData.EnsureData(InItems, InArchives);
	EquipItem(InEquipment);
}

UInventoryWidget* UInventoryComponent::GetWidget()
{
	if (WidgetPtr) return WidgetPtr;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		WidgetPtr = Manager->FindWidget<UInventoryWidget>();
	}
	return WidgetPtr;
}
