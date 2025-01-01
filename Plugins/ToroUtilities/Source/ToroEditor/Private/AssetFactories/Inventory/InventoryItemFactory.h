// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Inventory/InventoryItemData.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "InventoryItemFactory.generated.h"

UCLASS()
class UItemDataFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UItemDataFactory()
	{
		AssetName = TEXT("InventoryItem");
		SupportedClass = UInventoryItemData::StaticClass();
	}
};