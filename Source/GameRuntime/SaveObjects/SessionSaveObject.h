// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "SaveSystem/ToroSaveGame.h"
#include "DataTypes/GameplayRecord.h"
#include "SessionSaveObject.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class USessionSaveObject final : public UToroSaveGame
{
	GENERATED_BODY()

	friend class ASessionState;

public:

	USessionSaveObject()
	{
		SaveName = TEXT("SessionData");
	}

private:

	UPROPERTY(SaveGame)
		TSet<FName> Flags;

	UPROPERTY(SaveGame)
		TArray<FName> Archives;

	UPROPERTY(SaveGame)
		FGameplayRecord Session;
};
