// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "SaveSystem/ToroSaveGame.h"
#include "SaveSystem/ToroSaveManager.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
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

	UFUNCTION(BlueprintPure, Category = GameSave)
	void AddGameFlag(const FGameplayTag& InFlag, const bool bNotify = false)
	{
		if (InFlag.IsValid() && !GameFlags.Contains(InFlag))
		{
			GameFlags.Add(InFlag);
			if (bNotify)
			{
				const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem::
				   GetSharedMessageSystem<FAsyncGameplayMessageSystem>(FWorldGetter::Get(this));
				System->QueueMessageForBroadcast(FAsyncMessageId(InFlag));
			}
		}
	}

	UFUNCTION(BlueprintPure, Category = GameSave)
	bool HasGameFlag(const FGameplayTag& InFlag) const
	{
		return InFlag.IsValid() && GameFlags.Contains(InFlag);
	}

private:

	UPROPERTY(SaveGame)
		int32 PlayTime;

	UPROPERTY(SaveGame)
		TSet<FGameplayTag> GameFlags;
};
