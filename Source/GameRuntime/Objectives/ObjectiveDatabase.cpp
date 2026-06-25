// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "ObjectiveDatabase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Objective, "Objective")

FObjectiveDbEntry UObjectiveDatabase::GetObjectiveData(const FGameplayTag& Key)
{
	if (const FObjectiveDbEntry* Entry = GetEntry(Key))
	{
		return *Entry;
	}

	return FObjectiveDbEntry();
}

const FObjectiveDbEntry* UObjectiveDatabase::GetEntry(const FGameplayTag& Key)
{
	return Database.Get() ? Database->GetValue<FObjectiveDbEntry>(Key) : nullptr;
}
