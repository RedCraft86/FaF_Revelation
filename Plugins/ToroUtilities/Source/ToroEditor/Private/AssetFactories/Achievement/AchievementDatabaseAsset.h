// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Achievements/AchievementTypes.h"
#include "AssetFactories/ToroDatabaseTypeActions.h"

class FAchievementDatabaseAsset final : public FToroDatabaseTypeActions
{
public:

	explicit FAchievementDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(200, 200, 10); }
	virtual UClass* GetSupportedClass() const override { return UAchievementDatabase::StaticClass(); }
	virtual FText GetName() const override
	{
		return NSLOCTEXT("ToroEditor", "AchievementDatabaseName", "Achievement Database");
	}
};