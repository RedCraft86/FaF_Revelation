// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObject.h"
#include "ToroSaveTypes.generated.h"

UCLASS()
class TOROUTILITIES_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGlobalSave() {}

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, FDateTime> Endings;
	
	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, uint8> Achievements;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Content;

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
	virtual void SaveObject(const TFunction<void(const ESaveGameError)>& Callback) override;
	virtual void BeginTimeCache() { CachedTime = FDateTime::Now(); }

protected:

	UPROPERTY() FDateTime CachedTime;
	virtual void SerializeData(FArchive& Ar) override { Ar << PlayTime; }
};
