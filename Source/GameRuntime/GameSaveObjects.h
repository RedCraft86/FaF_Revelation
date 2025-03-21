// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SaveSystem/ToroSaveTypes.h"
#include "Inventory/InventoryComponent.h"
#include "GameSaveObjects.generated.h"

UCLASS()
class GAMERUNTIME_API UGlobalSaveObject final : public UToroGlobalSave
{
	GENERATED_BODY()

public:

	UGlobalSaveObject() {}

	UPROPERTY(BlueprintReadOnly, Category = GlobalSave)
		TSet<FGuid> SectionNodes;

protected:

	virtual void SerializeData(FArchive& Ar) override
	{
		Super::SerializeData(Ar);
		Ar << SectionNodes;
	}
};

UCLASS()
class GAMERUNTIME_API UGameSaveObject final : public UToroGameSave
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