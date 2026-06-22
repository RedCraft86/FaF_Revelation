// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "SaveSystem/ToroSaveGame.h"
#include "SaveSystem/ToroSaveManager.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
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

	UFUNCTION(BlueprintPure, Category = GlobalFlag)
	void AddGlobalFlag(const FGameplayTag& InFlag)
	{
		if (InFlag.IsValid() && !GlobalFlags.Contains(InFlag))
		{
			GlobalFlags.Add(InFlag);

			const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem::
				GetSharedMessageSystem<FAsyncGameplayMessageSystem>(FWorldGetter::Get(this));
			System->QueueMessageForBroadcast(FAsyncMessageId(InFlag));
		}
	}

	UFUNCTION(BlueprintPure, Category = GlobalFlag)
	bool HasGlobalFlag(const FGameplayTag& InFlag) const
	{
		return InFlag.IsValid() && GlobalFlags.Contains(InFlag);
	}

private:

	UPROPERTY(SaveGame)
		TSet<FGameplayTag> GlobalFlags;
};
