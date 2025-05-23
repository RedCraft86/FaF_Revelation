// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameState.h"

AToroGameState::AToroGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

bool AToroGameState::IsCharacterInZone(const FGameplayTag Character, const FGameplayTag Zone)
{
	return CharacterToZone.FindRef(Character) == Zone;
}

FGameplayTag AToroGameState::GetCharacterZone(const FGameplayTag Character)
{
	return CharacterToZone.FindRef(Character);
}

FGameplayTagContainer AToroGameState::GetAllCharactersInZone(const FGameplayTag Zone)
{
	FGameplayTagContainer Result;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : CharacterToZone)
	{
		if (Pair.Value == Zone) Result.AddTag(Pair.Key);
	}
	return Result;
}
