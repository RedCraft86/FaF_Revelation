// Copyright (C) RedCraft86. All Rights Reserved.

#include "InventoryComponent.h"
#include "Framework/ToroWidgetManager.h"
#include "Player/GamePlayerChar.h"
#include "InventoryWidget.h"

#define InvItems InvData.Items
#define InvArchives InvData.Archives
#define InvEquipment InvData.Equipment

UInventoryComponent::UInventoryComponent(): bInInventory(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UInventoryComponent* UInventoryComponent::Get(const UObject* ContextObject)
{
	AGamePlayerChar* Player = AGamePlayerChar::Get<AGamePlayerChar>(ContextObject);
	return Player ? Player->Inventory : nullptr;
}

uint8 UInventoryComponent::AddItem(const TSoftObjectPtr<UInvItemData>& InItem, const uint8 Amount)
{
	if (Amount == 0) return 0;
	const UInvItemData* Item = InItem.LoadSynchronous();
	if (!Item) return Amount;
	
	if (Item->ItemType == EInvItemType::Archive)
	{
		// Assume we want the secret to be found
		AddArchive(InItem, true);
		return 0;
	}

	uint8 Overflow = Amount;
	if (Item->ItemType == EInvItemType::Generic)
	{
		if (InvItems.Contains(InItem))
		{
			const int32 Target = InvItems.FindRef(InItem) + (int32)Amount;
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

uint8 UInventoryComponent::RemoveItem(const TSoftObjectPtr<UInvItemData>& InItem, const uint8 Amount)
{
	if (Amount == 0) return 0;
	const UInvItemData* Item = InItem.LoadSynchronous();
	if (!Item || Item->ItemType == EInvItemType::Archive) return Amount;

	if (InvEquipment == InItem)
	{
		UnEquipItem();
	}

	uint8 Missing = Amount;
	if (Item->ItemType == EInvItemType::Generic)
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

void UInventoryComponent::AddArchive(const TSoftObjectPtr<UInvItemData>& InArchive, const bool bSecretFound)
{
	const UInvItemData* Archive = InArchive.LoadSynchronous();
	if (!Archive || Archive->ItemType != EInvItemType::Archive) return;
	if (!InvArchives.Contains(InArchive) || !InvArchives.FindRef(InArchive).bSecretFound)
	{
		InvArchives.Add(InArchive, {(uint8)InvArchives.Num(), bSecretFound});
	}
}

void UInventoryComponent::GetArchiveState(const TSoftObjectPtr<UInvItemData>& InArchive, bool& bDiscovered, bool& bSecretFound) const
{
	const UInvItemData* Archive = InArchive.LoadSynchronous();
	if (!Archive || Archive->ItemType != EInvItemType::Archive) return;

	bDiscovered = InvArchives.Contains(InArchive);
	if (bDiscovered) bSecretFound = InvArchives.FindRef(InArchive).bSecretFound;
}

uint8 UInventoryComponent::GetItemAmount(const TSoftObjectPtr<UInvItemData>& InItem) const
{
	return InvArchives.Contains(InItem) ? 1 : InvItems.FindRef(InItem);
}

bool UInventoryComponent::HasItem(const TSoftObjectPtr<UInvItemData>& InItem, const uint8 MinAmount) const
{
	return GetItemAmount(InItem) >= MinAmount;
}

void UInventoryComponent::UnEquipItem()
{
	InvEquipment.Reset();
	if (EquipActor)
	{
		EquipActor->OnUnquip();
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

void UInventoryComponent::EquipItem(const TSoftObjectPtr<UInvItemData>& InItem)
{
	if (!InItem.LoadSynchronous()) return;
	if (InvEquipment.LoadSynchronous() || EquipActor)
	{
		UnEquipItem();
	}

	if (const UInvItemData* Item = InItem.LoadSynchronous();
		Item && Item->ItemType == EInvItemType::Equipment && !Item->Equipment.IsNull())
	{
		if (AInventoryEquipment* NewActor = GetWorld()->SpawnActor<AInventoryEquipment>(Item->Equipment.LoadSynchronous()))
		{
			InvEquipment = InItem;
			EquipActor = NewActor;
			EquipActor->OnEquip();
			EquipActor->AttachToComponent(Player->EquipmentRoot,
					FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

TArray<TSoftObjectPtr<UInvItemData>> UInventoryComponent::GetSortedItems()
{
	TArray<TSoftObjectPtr<UInvItemData>> Items;
	for (auto It = InvItems.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull() || It.Value() == 0) It.RemoveCurrent();
		else Items.Add(It.Key());
	}

	Items.Sort([](const TSoftObjectPtr<UInvItemData>& A, const TSoftObjectPtr<UInvItemData>& B)
	{
		const UInvItemData *ItemA = A.LoadSynchronous(), *ItemB = B.LoadSynchronous();
		return ItemA->Priority != ItemB->Priority
			? ItemA->Priority < ItemB->Priority
			: ItemA->DisplayName.CompareTo(ItemB->DisplayName) < 0;
	});

	return Items;
}

TArray<TSoftObjectPtr<UInvItemData>> UInventoryComponent::GetSortedArchives()
{
	TArray<TSoftObjectPtr<UInvItemData>> Archives;
	for (auto It = InvArchives.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull()) It.RemoveCurrent();
		else Archives.Add(It.Key());
	}

	Archives.Sort([this](const TSoftObjectPtr<UInvItemData>& A, const TSoftObjectPtr<UInvItemData>& B)
	{
		return InvArchives[A].EntryNum > InvArchives[B].EntryNum;
	});

	return Archives;
}

void UInventoryComponent::LoadSaveData(const FInventoryData& InData)
{
	InvData.Items = InData.Items;
	InvData.Archives = InData.Archives;
	EquipItem(InData.Equipment.LoadSynchronous());
}

void UInventoryComponent::EnsureInventory(const TSoftObjectPtr<UInvItemData>& InEquipment,
	const TMap<TSoftObjectPtr<UInvItemData>, uint8>& InItems,
	const TMap<TSoftObjectPtr<UInvItemData>, bool>& InArchives)
{
	for (const TPair<TSoftObjectPtr<UInvItemData>, uint8>& Item : InItems)
	{
		if (const UInvItemData* ItemData = Item.Key.LoadSynchronous())
		{
			InvItems.FindOrAdd(Item.Key) = FMath::Max(ItemData->StackSize, Item.Value);
		}
	}

	for (const TPair<TSoftObjectPtr<UInvItemData>, bool>& Archive : InArchives)
	{
		if (Archive.Key.IsNull()) continue;
		InvArchives.Add(Archive.Key, {(uint8)InvArchives.Num(), Archive.Value});
	}

	EquipItem(InEquipment);
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
		if (AToroPlayerController* PC = Player->GetPlayerController())
		{
			PC->AddPauseRequest(this);
			CachedInputMode = PC->GetInputModeData();
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
		if (AToroPlayerController* PC = Player->GetPlayerController())
		{
			PC->RemovePauseRequest(this);
			PC->SetInputModeData(CachedInputMode);
		}
	}
}

void UInventoryComponent::SetEquipmentUse(const bool bUsing) const
{
	if (EquipActor) EquipActor->SetUseState(bUsing);
}

UInventoryWidget* UInventoryComponent::GetWidget()
{
	if (Widget) return Widget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Widget = Manager->FindWidget<UInventoryWidget>();
	}
	return Widget;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = GetOwner<AGamePlayerChar>();
}
