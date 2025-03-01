// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/FootstepTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "FootstepDBFactory.generated.h"

UCLASS()
class TOROEDITOR_API UFootstepDBFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UFootstepDBFactory()
	{
		AssetName = TEXT("FootstepDatabase");
		SupportedClass = UFootstepDatabase::StaticClass();
	}
};
