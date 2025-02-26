// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/FootstepTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "FootstepDatabaseFactory.generated.h"

UCLASS()
class TOROEDITOR_API UFootstepDatabaseFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UFootstepDatabaseFactory()
	{
		AssetName = TEXT("FootstepDatabase");
		SupportedClass = UFootstepDatabase::StaticClass();
	}
};
