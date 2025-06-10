// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZones/LevelZoneManager.h"
#include "Actors/ToroCharacter.h"

namespace ZoneTags
{
	DEFINE_GAMEPLAY_TAG(Zone)
}

bool ULevelZoneManager::IsCharacterInZone(const FGameplayTag Character, const FGameplayTag Zone)
{
	return CharToZone.FindRef(Character) == Zone;
}

FGameplayTag ULevelZoneManager::GetZoneByCharacter(const FGameplayTag Character)
{
	return CharToZone.FindRef(Character);
}

FGameplayTagContainer ULevelZoneManager::GetCharactersInZone(const FGameplayTag Zone)
{
	FGameplayTagContainer Characters;
	if (!ZoneTags::IsValidTag(Zone)) return Characters;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : CharToZone)
	{
		if (Pair.Value == Zone) Characters.AddTag(Pair.Key);
	}
	return Characters;
}

void ULevelZoneManager::AssignCharToZone(const FGameplayTag& Char, const FGameplayTag& Zone)
{
	if (CharacterTags::IsValidTag(Char) && ZoneTags::IsValidTag(Zone))
	{
		const FGameplayTag LastZone = GetZoneByCharacter(Char);
		if (Zone == LastZone) return;

		if (LastZone.IsValid())
		{
			OnExitZone.Broadcast(Char, LastZone);
		}

		CharToZone.Add(Char, Zone);
		OnEnterZone.Broadcast(Char, Zone);
	}
}
