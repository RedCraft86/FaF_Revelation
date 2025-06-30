// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "ToroGlobalSave.generated.h"

namespace ContentTags
{
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Content)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Backgrounds)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Character)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Ending)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Mode)
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Misc)

	TOROUTILITIES_API inline bool IsValidTag(const FGameplayTag& InTag)
	{
		return InTag.IsValid()
			&& InTag != TAG_Content
			&& InTag != TAG_Backgrounds
			&& InTag != TAG_Character
			&& InTag != TAG_Ending
			&& InTag != TAG_Mode
			&& InTag != TAG_Misc;
	}
}

UCLASS()
class TOROUTILITIES_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Content;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGuid> Cutscenes;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};
