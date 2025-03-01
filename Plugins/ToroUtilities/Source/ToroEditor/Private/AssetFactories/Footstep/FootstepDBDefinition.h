// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/FootstepTypes.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "FootstepDBDefinition.generated.h"

UCLASS()
class TOROEDITOR_API UFootstepDBDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:
	
	UFootstepDBDefinition() {}
	virtual FLinearColor GetAssetColor() const override { return FColor(150, 120, 100); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UFootstepDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "FootstepDatabaseName", "Footstep Database");
	}
};