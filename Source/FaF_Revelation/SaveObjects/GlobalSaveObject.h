// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "SaveSystem/ToroSaveGame.h"
#include "SaveSystem/ToroSaveManager.h"
#include "GlobalSaveObject.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class FAF_REVELATION_API UGlobalSaveObject final : public UToroSaveGame
{
	GENERATED_BODY()

public:

	UGlobalSaveObject()
	{
		SaveName = TEXT("GlobalData");
	}

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject, DisplayName = "Get Global Save"))
	[[nodiscard]] static UGlobalSaveObject* Get(const UObject* ContextObject)
	{
		UToroSaveManager* SM = UToroSaveManager::Get(ContextObject);
		return IsValid(SM) ? SM->GetOrCreateSaveObject<UGlobalSaveObject>() : nullptr;
	}

	UPROPERTY(SaveGame)
		TSet<FGameplayTag> GlobalFlags;
};
