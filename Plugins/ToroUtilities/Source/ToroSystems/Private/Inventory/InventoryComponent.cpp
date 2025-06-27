// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"

#define InvItems InvData.Items
#define InvArchives InvData.Archives
#define InvEquipment InvData.Equipment

UInventoryComponent::UInventoryComponent(): bInInventory(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
