// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/LocalSoundTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "LocalSoundDatabaseFactory.generated.h"

UCLASS()
class TOROEDITOR_API ULocalSoundDatabaseFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	ULocalSoundDatabaseFactory()
	{
		AssetName = TEXT("LocalSoundDatabase");
		SupportedClass = ULocalSoundDatabase::StaticClass();
	}
};
