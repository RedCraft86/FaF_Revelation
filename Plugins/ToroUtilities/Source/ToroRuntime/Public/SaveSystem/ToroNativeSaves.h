// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "Helpers/GameplayTagMacros.h"
#include "ToroNativeSaves.generated.h"

UCLASS()
class TORORUNTIME_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGlobalSave();

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGuid> Cutscenes;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, uint8> Achievements;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Content;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};

UCLASS()
class TORORUNTIME_API UToroGameSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGameSave();

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		float PlayTime;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};