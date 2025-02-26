// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GuidePages/GuidePageTypes.h"
#include "AssetFactories/ToroDatabaseTypeActions.h"

class FGuidePageDatabaseAsset final : public FToroDatabaseTypeActions
{
public:

	explicit FGuidePageDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(20, 150, 120); }
	virtual UClass* GetSupportedClass() const override { return UGuidePageDatabase::StaticClass(); }
	virtual FText GetName() const override
	{
		return NSLOCTEXT("ToroEditor", "GuidePageDatabaseName", "Local Sound Database");
	}
};