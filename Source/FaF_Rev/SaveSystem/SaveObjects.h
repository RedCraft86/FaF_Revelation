﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DifficultyData.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventoryComponent.h"
#include "SaveObjects.generated.h"

UENUM(BlueprintInternalUseOnly)
enum class ESaveGameError : uint8
{
	None,
	Compress,
	Decompress,
	FileWrite,
	FileRead,
};

UCLASS(Abstract)
class FAF_REV_API USaveObjectBase : public UObject
{
	GENERATED_BODY()

public:

	USaveObjectBase() : LastError(ESaveGameError::None) {}

	UFUNCTION(BlueprintPure, Category = "SaveObject")
		ESaveGameError GetLastError() const { return LastError; }

	FString GetSavePath() const;
	void SaveToFile(const TFunction<void(const ESaveGameError)>& Callback);
	void LoadFromFile(const TFunction<void(const ESaveGameError)>& Callback);
	virtual void DeleteFile();
	
protected:

	ESaveGameError LastError;

	virtual void SerializeData(FArchive& Ar) {}
	virtual FString GetSaveFileName() const { return TEXT("Default"); }
};

UCLASS(BlueprintType, NotBlueprintable)
class FAF_REV_API UGameSaveObject final : public USaveObjectBase
{
	GENERATED_BODY()

public:

	UGameSaveObject() : Difficulty(EDifficultyMode::Normal), PlayTime(0.0f), Sequence({}), Inventory({}) {}

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		EDifficultyMode Difficulty;
	
	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TSet<FGameplayTag> TransientKeys;

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		float PlayTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TMap<FGuid, FInventorySaveData> Inventory;

	virtual void DeleteFile() override;

private:

	virtual void SerializeData(FArchive& Ar);
	virtual FString GetSaveFileName() const { return TEXT("Game"); }
};

UCLASS(BlueprintType, NotBlueprintable)
class FAF_REV_API UGlobalSaveObject final : public USaveObjectBase
{
	GENERATED_BODY()

public:

	UGlobalSaveObject() {}

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TSet<FName> Achievements;

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TMap<FGameplayTag, FDateTime> Endings;

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TSet<FGameplayTag> Menus;

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TSet<FGameplayTag> Content;

	UPROPERTY(BlueprintReadOnly, Category = "SaveObject")
		TSet<FGuid> SectionNodes;
	
	virtual void DeleteFile() override;

private:

	virtual void SerializeData(FArchive& Ar);
	virtual FString GetSaveFileName() const { return TEXT("Global"); }
};
