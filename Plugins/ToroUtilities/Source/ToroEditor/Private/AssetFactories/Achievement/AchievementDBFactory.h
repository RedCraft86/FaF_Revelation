// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Achievements/AchievementTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AchievementDBFactory.generated.h"

UCLASS()
class TOROEDITOR_API UAchievementDBFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UAchievementDBFactory()
	{
		AssetName = TEXT("AchievementDatabase");
		SupportedClass = UAchievementDatabase::StaticClass();
	}
};
