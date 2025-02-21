// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GuidePages/GuidePageTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "GuidePageDatabaseFactory.generated.h"

UCLASS()
class TOROEDITOR_API UGuidePageDatabaseFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UGuidePageDatabaseFactory()
	{
		AssetName = TEXT("GuidePageDatabase");
		SupportedClass = UGuidePageDatabase::StaticClass();
	}
};
