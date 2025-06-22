// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerState.h"
#include "Helpers/ClassGetters.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UInventoryComponent* UInventoryComponent::GetInventory(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UInventoryComponent)
}
