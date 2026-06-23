// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "ObjectiveDatabase.h"
#include "ToroSettings.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Objective, "Objective")

UObjectiveDatabase* UObjectiveDatabase::Get()
{
	return UToroSettings::Get()->GetDatabase<UObjectiveDatabase>();
}

const FObjectiveEntry* UObjectiveDatabase::GetEntry(const FGameplayTag& Key)
{
	const UObjectiveDatabase* Database = UObjectiveDatabase::Get();
	return Database ? Database->GetValue<FObjectiveEntry>(Key) : nullptr;
}
