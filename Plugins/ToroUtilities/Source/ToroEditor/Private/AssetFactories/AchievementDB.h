// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Achievements/AchievementTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "AchievementDB.generated.h"

UCLASS()
class TOROEDITOR_API UAchievementDBFactory final : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UAchievementDBFactory()
	{
		AssetName = TEXT("AchievementDatabase");
		SupportedClass = UAchievementDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UAchievementDBDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(200, 200, 10); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UAchievementDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "AchievementDatabaseName", "Achievement Database");
	}
};