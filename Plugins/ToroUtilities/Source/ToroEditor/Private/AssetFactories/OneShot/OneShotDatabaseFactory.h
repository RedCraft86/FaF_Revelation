// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/OneShotDataTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "OneShotDatabaseFactory.generated.h"

UCLASS()
class TOROEDITOR_API UOneShotDatabaseFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	UOneShotDatabaseFactory()
	{
		AssetName = TEXT("OneShotDatabase");
		SupportedClass = UOneShotDatabase::StaticClass();
	}
};
