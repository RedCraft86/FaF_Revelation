// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "ToroNativeSaves.generated.h"

namespace SaveTags
{
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(GlobalSave)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(GameSave)
}

UCLASS()
class TOROUTILITIES_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGlobalSave() {}

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

	UToroGameSave() {}
};
