// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MusicSystem/LocalSoundTypes.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "LocalSoundDBFactory.generated.h"

UCLASS()
class TOROEDITOR_API ULocalSoundDBFactory : public UToroAssetFactory
{
	GENERATED_BODY()
	
public:

	ULocalSoundDBFactory()
	{
		AssetName = TEXT("LocalSoundDatabase");
		SupportedClass = ULocalSoundDatabase::StaticClass();
	}
};
