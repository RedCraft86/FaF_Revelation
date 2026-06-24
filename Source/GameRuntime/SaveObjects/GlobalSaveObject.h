// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "SaveSystem/ToroSaveGame.h"
#include "DataTypes/GameplayRecord.h"
#include "GlobalSaveObject.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UGlobalSaveObject final : public UToroSaveGame
{
	GENERATED_BODY()

	friend class AGlobalState;

public:

	UGlobalSaveObject()
	{
		SaveName = TEXT("GlobalData");
	}

private:

	UPROPERTY(SaveGame)
		TSet<FName> Flags;

	UPROPERTY(SaveGame)
		TArray<FGameplayRecord> Sessions;
};
