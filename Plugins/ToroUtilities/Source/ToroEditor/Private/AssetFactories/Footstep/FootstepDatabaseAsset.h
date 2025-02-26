// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/FootstepTypes.h"
#include "AssetFactories/ToroDatabaseTypeActions.h"

class FFootstepDatabaseAsset final : public FToroDatabaseTypeActions
{
public:

	explicit FFootstepDatabaseAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(150, 120, 100); }
	virtual UClass* GetSupportedClass() const override { return UFootstepDatabase::StaticClass(); }
	virtual FText GetName() const override
	{
		return NSLOCTEXT("ToroEditor", "FootstepDatabaseName", "Footstep Database");
	}
};