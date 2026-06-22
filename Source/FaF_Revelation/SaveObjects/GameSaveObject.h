// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "FaFRevSaveGame.h"
#include "SaveSystem/ToroSaveManager.h"
#include "GameSaveObject.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class FAF_REVELATION_API UGameSaveObject final : public UFaFRevSaveGame
{
	GENERATED_BODY()

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

	void InitPlayTimer(const UWorld* World)
	{
		if (World && !TimerWorld.IsValid())
		{
			TimerWorld = World;
			TimerWorld->GetTimerManager().SetTimer(PlayTimeTimer, 
				this, &UGameSaveObject::CountPlayTime, 1.0f, true);
		}
	}

private:

	UPROPERTY(SaveGame)
		int32 PlayTime;

	FTimerHandle PlayTimeTimer;
	TWeakObjectPtr<const UWorld> TimerWorld;

	void CountPlayTime()
	{
		if (CurrentOperation == EToroSaveOperation::None)
		{
			PlayTime++;
		}
	}

	virtual void BeginDestroy() override
	{
		if (TimerWorld.IsValid())
		{
			TimerWorld->GetTimerManager().ClearTimer(PlayTimeTimer);
		}

		Super::BeginDestroy();
	}
};
