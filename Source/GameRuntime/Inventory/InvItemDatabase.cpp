// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InvItemDatabase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_InventoryItem, "Inventory.Item")
UE_DEFINE_GAMEPLAY_TAG(TAG_InvEquipment, "Inventory.Item.Equipment")
UE_DEFINE_GAMEPLAY_TAG(TAG_InvMission, "Inventory.Item.Mission")
UE_DEFINE_GAMEPLAY_TAG(TAG_InvKey, "Inventory.Item.Key")

FInvItemDbEntry UInvItemDatabase::GetItemData(const FGameplayTag& Key)
{
	if (const FInvItemDbEntry* Entry = GetEntry(Key))
	{
		return *Entry;
	}

	return FInvItemDbEntry();
}

const FInvItemDbEntry* UInvItemDatabase::GetEntry(const FGameplayTag& Key)
{
	return Database.Get() ? Database->GetValue<FInvItemDbEntry>(Key) : nullptr;
}