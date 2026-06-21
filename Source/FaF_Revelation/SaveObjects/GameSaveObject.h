// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "SaveSystem/ToroSaveGame.h"
#include "SaveSystem/ToroSaveManager.h"
#include "GameSaveObject.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class FAF_REVELATION_API UGameSaveObject final : public UToroSaveGame
{
	GENERATED_BODY()

	friend class AFaFGameState;

public:

	UGameSaveObject()
	{
		SaveName = TEXT("GameData");
	}

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject, DisplayName = "Get Game Save"))
	[[nodiscard]] static UGameSaveObject* Get(const UObject* ContextObject)
	{
		UToroSaveManager* SM = UToroSaveManager::Get(ContextObject);
		return IsValid(SM) ? SM->GetOrCreateSaveObject<UGameSaveObject>() : nullptr;
	}

	UPROPERTY(SaveGame)
		int32 PlayTime;

	UPROPERTY(SaveGame)
		TSet<FGameplayTag> GameFlags;
};
