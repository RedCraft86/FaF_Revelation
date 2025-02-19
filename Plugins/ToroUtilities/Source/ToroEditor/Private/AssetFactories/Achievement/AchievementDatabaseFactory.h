// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Achievements/AchievementTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AchievementDatabaseFactory.generated.h"

UCLASS()
class TOROEDITOR_API UAchievementDatabaseFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UAchievementDatabaseFactory()
	{
		AssetName = TEXT("AchievementDatabase");
		SupportedClass = UAchievementDatabase::StaticClass();
	}
};
