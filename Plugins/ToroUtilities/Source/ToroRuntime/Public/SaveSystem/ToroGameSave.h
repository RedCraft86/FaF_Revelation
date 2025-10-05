// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventoryAsset.h"
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
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TArray<TSoftObjectPtr<UInventoryAsset>> Archives;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TMap<FGameplayTag, FInventoryItems> Items;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TSoftObjectPtr<UInventoryAsset> Equipment;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};