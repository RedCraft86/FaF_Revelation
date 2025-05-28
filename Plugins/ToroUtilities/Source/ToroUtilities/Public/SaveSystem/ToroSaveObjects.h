// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SaveGameTypes.h"
#include "ToroSaveObjects.generated.h"

DECLARE_GAMEPLAY_TAG_CHILD(Save, Global)
DECLARE_GAMEPLAY_TAG_CHILD(Save, Game)

UCLASS()
class TOROUTILITIES_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGlobalSave() {}

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Content;
	
	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, uint8> Achievements;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};

UCLASS()
class TOROUTILITIES_API UToroGameSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGameSave() : PlayTime(0.0f) {}

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		int64 PlayTime;

	virtual void DeleteData() override;
	virtual void SaveObject(const TFunction<void(const ESaveGameStatus)>& Callback) override;
	virtual void BeginTimeCache() { CachedTime = FDateTime::Now(); } // Call when entering game

protected:

	UPROPERTY() FDateTime CachedTime = 0;
	virtual void SerializeData(FArchive& Ar) override;
};