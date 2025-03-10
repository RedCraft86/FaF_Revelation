﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Inventory/InventoryComponent.h"
#include "SaveSystem/BaseSaveObjects.h"
#include "GameSaveObject.generated.h"

UCLASS(DisplayName = "FaF Rev Save")
class UGameSaveObject final : public UGameSaveObjectBase
{
	GENERATED_BODY()

public:

	UGameSaveObject() {}

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TMap<FGuid, FInvSaveData> Inventory;

	virtual void DeleteData() override
	{
		Super::DeleteData();
		Sequence.Empty();
		Inventory.Empty();
	}

protected:

	virtual void SerializeData(FArchive& Ar) override
	{
		Super::SerializeData(Ar);
		Ar << Sequence;
		Ar << Inventory;
	}
};