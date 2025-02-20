// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObject.h"
#include "Inventory/InventoryComponent.h"
#include "BaseSaveObjects.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GlobalSave);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GameSave);

UCLASS()
class TORORUNTIME_API UGlobalSaveObjectBase : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UGlobalSaveObjectBase() {}

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, FDateTime> Endings;
	
	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, uint8> Achievements;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Content;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGuid> Sections;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};

UCLASS()
class TORORUNTIME_API UGameSaveObjectBase : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UGameSaveObjectBase() : PlayTime(0.0f) {}

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		int64 PlayTime;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TMap<FGuid, FInventorySaveData> Inventory;

	virtual void DeleteData() override;
	virtual void OnCreation() override;
	virtual void SaveObject(const TFunction<void(const ESaveGameError)>& Callback) override;

protected:

	UPROPERTY() FDateTime CachedTime;
	virtual void SerializeData(FArchive& Ar) override;
};
