// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MusicSystem/LocalSoundTypes.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "LocalSoundDBDefinition.generated.h"

UCLASS()
class TOROEDITOR_API ULocalSoundDBDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:
	
	ULocalSoundDBDefinition() {}
	virtual FLinearColor GetAssetColor() const override { return FColor(160, 25, 100); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return ULocalSoundDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "LocalSoundDatabaseName", "Local Sound Database");
	}
};