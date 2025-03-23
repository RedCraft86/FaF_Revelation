// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GuidePages/GuidePageTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "GuidePageDB.generated.h"

UCLASS()
class TOROEDITOR_API UGuidePageDBFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UGuidePageDBFactory()
	{
		AssetName = TEXT("GuidePageDatabase");
		SupportedClass = UGuidePageDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UGuidePageDBDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(20, 150, 120); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UGuidePageDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "GuidePageDatabaseName", "Local Sound Database");
	}
};