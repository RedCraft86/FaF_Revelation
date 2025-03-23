// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/FootstepTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "FootstepDB.generated.h"

UCLASS()
class TOROEDITOR_API UFootstepDBFactory final : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UFootstepDBFactory()
	{
		AssetName = TEXT("FootstepDatabase");
		SupportedClass = UFootstepDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UFootstepDBDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(150, 120, 100); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UFootstepDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "FootstepDatabaseName", "Footstep Database");
	}
};