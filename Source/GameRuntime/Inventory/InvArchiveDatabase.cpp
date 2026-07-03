// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InvArchiveDatabase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_InventoryArchive, "Inventory.Archive")

FInvArchiveDbEntry UInvArchiveDatabase::GetArchiveData(const FGameplayTag& Key)
{
	if (const FInvArchiveDbEntry* Entry = GetEntry(Key))
	{
		return *Entry;
	}

	return FInvArchiveDbEntry();
}

const FInvArchiveDbEntry* UInvArchiveDatabase::GetEntry(const FGameplayTag& Key)
{
	return Database.Get() ? Database->GetValue<FInvArchiveDbEntry>(Key) : nullptr;
}