// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventorySlotData.h"
#include "ToroGameSave.generated.h"

UCLASS()
class TORORUNTIME_API UToroGameSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGameSave();

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		float PlayTime;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		FInventoryArchives Archives;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TMap<FGameplayTag, FInventoryItems> Items;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};