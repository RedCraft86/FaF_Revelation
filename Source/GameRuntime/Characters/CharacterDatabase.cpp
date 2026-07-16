// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "CharacterDatabase.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Character, "Char")

FCharacterDbEntry UCharacterDatabase::GetCharacterData(const FGameplayTag& Key)
{
	if (const FCharacterDbEntry* Entry = GetEntry(Key))
	{
		return *Entry;
	}

	return FCharacterDbEntry();
}

const FCharacterDbEntry* UCharacterDatabase::GetEntry(const FGameplayTag& Key)
{
	return Database.Get() ? Database->GetValue<FCharacterDbEntry>(Key) : nullptr;
}
