// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/AssetUserData.h"
#include "AssetTagData.generated.h"

UCLASS(MinimalAPI)
class UAssetTagData : public UAssetUserData
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Asset, AssetRegistrySearchable)
		TSet<FString> Tags;
#endif
};
