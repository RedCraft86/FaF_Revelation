// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GuidePages/GuidePageTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "GuidePageDBFactory.generated.h"

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
