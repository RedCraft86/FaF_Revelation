// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/OneShotDataTypes.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FOneShotDatabaseAssetTypeActions final : public FToroAssetTypeActions
{
public:

	explicit FOneShotDatabaseAssetTypeActions() {}
	virtual FColor GetTypeColor() const override { return FColor(160, 25, 100); }
	virtual UClass* GetSupportedClass() const override { return UOneShotDatabase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "OneShotDatabaseName", "One Shot Database"); }
};