// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "Helpers/GameplayTagMacros.h"
#include "ToroNativeSaves.generated.h"

namespace ContentTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Content)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Cutscene)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Tutorial)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Character)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Ending)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Misc)

	TORORUNTIME_API inline bool IsValidTag(const FGameplayTag& InTag)
	{
		static FGameplayTag BaseTag = TAG_Content.GetTag();
		return InTag.MatchesTag(BaseTag)
			&& InTag != TAG_Content
			&& InTag != TAG_Cutscene
			&& InTag != TAG_Tutorial
			&& InTag != TAG_Character
			&& InTag != TAG_Ending
			&& InTag != TAG_Misc;
	}
}

UCLASS()
class TORORUNTIME_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGlobalSave();

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