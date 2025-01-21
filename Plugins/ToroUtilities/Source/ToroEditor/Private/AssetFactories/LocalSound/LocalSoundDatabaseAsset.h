// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/LocalSoundTypes.h"
#include "AssetFactories/ToroAssetTypeActions.h"

class FLocalSoundDatabaseAsset final : public FToroAssetTypeActions
{
public:

	explicit FLocalSoundDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(160, 25, 100); }
	virtual UClass* GetSupportedClass() const override { return ULocalSoundDatabase::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "LocalSoundDatabaseName", "Local Sound Database"); }
};