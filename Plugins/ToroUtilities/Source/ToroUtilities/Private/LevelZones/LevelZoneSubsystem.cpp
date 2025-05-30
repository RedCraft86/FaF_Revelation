// Copyright (C) RedCraft86. All Rights Reserved.

#include "LevelZones/LevelZoneSubsystem.h"
#include "Interfaces/CharInterface.h"

DEFINE_GAMEPLAY_TAG_BASE(Zone)

bool ULevelZoneSubsystem::IsCharacterInZone(const FGameplayTag Character, const FGameplayTag Zone)
{
	return CharToZone.FindRef(Character) == Zone;
}

FGameplayTag ULevelZoneSubsystem::GetZoneByCharacter(const FGameplayTag Character)
{
	return CharToZone.FindRef(Character);
}

FGameplayTagContainer ULevelZoneSubsystem::GetCharactersInZone(const FGameplayTag Zone)
{
	FGameplayTagContainer Characters;
	if (!VerifyZoneTag(Zone)) return Characters;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : CharToZone)
	{
		if (Pair.Value == Zone) Characters.AddTag(Pair.Key);
	}
	return Characters;
}

void ULevelZoneSubsystem::AssignCharToZone(const FGameplayTag& Char, const FGameplayTag& Zone)
{
	if (VerifyCharacterTag(Char) && VerifyZoneTag(Zone))
	{
		if (const FGameplayTag LastZone = GetZoneByCharacter(Char); LastZone.IsValid())
		{
			OnExitZone.Broadcast(Char, LastZone);
		}

		CharToZone.Add(Char, Zone);
		OnEnterZone.Broadcast(Char, Zone);
	}
}
