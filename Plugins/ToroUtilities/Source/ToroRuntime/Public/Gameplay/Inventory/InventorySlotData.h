// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryAsset.h"
#include "JsonObjectWrapper.h"
#include "InventorySlotData.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryItemSlot
{
	GENERATED_BODY()

	UPROPERTY() TSoftObjectPtr<UInventoryAsset> Asset;
	UPROPERTY() uint8 Amount;
	UPROPERTY() FString Metadata;

	FInventoryItemSlot(): Asset(nullptr), Amount(0), Metadata(TEXT("{}")) {}

	FInventoryItemSlot(UInventoryAsset* InAsset, const uint8 InAmount = 1)
		: Asset(InAsset), Amount(InAmount), Metadata(TEXT("{}"))
	{}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryItemSlot& Data)
	{
		Ar << Data.Asset;
		Ar << Data.Amount;
		Ar << Data.Metadata;
		return Ar;
	}

	FORCEINLINE bool IsValidSlot() const { return !Asset.IsNull() && Amount > 0; }
	FORCEINLINE UInventoryAsset* GetAsset() const { return Asset.LoadSynchronous(); }

	void AddAmount(const uint8 InAmount);
	bool TakeAmount(const uint8 InAmount);
	FJsonObjectWrapper GetJsonMetadata() const;
	void SetJsonMetadata(const FJsonObjectWrapper& InJson);
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryItems
{
	GENERATED_BODY()

private:

	UPROPERTY() TArray<FInventoryItemSlot> Slots;
	UPROPERTY() TSoftObjectPtr<UInventoryAsset> Equipped;
	
public:

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryItems& Data)
	{
		Ar << Data.Slots;
		Ar << Data.Equipped;
		return Ar;
	}

	void CleanupAndSort();
	void ForEachSlot(const TFunction<void(const FInventoryItemSlot&)>& Func);
	const FInventoryItemSlot* AddItem(UInventoryAsset* Item, const uint8 Amount = 1);
	bool TakeItem(const UInventoryAsset* Item, const uint8 Amount = 1);

	void SetEquippedItem(UInventoryAsset* Item);
	const UInventoryAsset* GetEquippedItem() const { return Equipped.LoadSynchronous(); }
	
	FInventoryItemSlot* FindItem(const UInventoryAsset* Item);
	const FInventoryItemSlot* FindItem(const UInventoryAsset* Item) const { return FindItem(Item); }
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryArchiveSlot
{
	GENERATED_BODY()

	UPROPERTY() TSoftObjectPtr<UInventoryAsset> Asset;
	UPROPERTY() bool bSecretFound;
	
	FInventoryArchiveSlot(): Asset(nullptr), bSecretFound(false) {}

	FInventoryArchiveSlot(UInventoryAsset* InAsset, const bool bKnowSecret = false)
		: Asset(InAsset), bSecretFound(bKnowSecret)
	{}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryArchiveSlot& Data)
	{
		Ar << Data.Asset;
		Ar << Data.bSecretFound;
		return Ar;
	}

	FORCEINLINE bool IsValidSlot() const { return !Asset.IsNull(); }
	FORCEINLINE UInventoryAsset* GetAsset() const { return Asset.LoadSynchronous(); }

	bool MarkSecretFound();
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryArchives
{
	GENERATED_BODY()

private:

	UPROPERTY() TArray<FInventoryArchiveSlot> Slots;
	
public:

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryArchives& Data)
	{
		Ar << Data.Slots;
		return Ar;
	}

	FORCEINLINE void Empty() { Slots.Empty(); }

	void ForEachSlot(const TFunction<void(const FInventoryArchiveSlot&)>& Func);
	const FInventoryArchiveSlot* AddArchive(UInventoryAsset* Archive, const bool bSecretFound = false);
	void MoveToLast(const int32 IndexToMove);
	
	FInventoryArchiveSlot* FindArchive(const UInventoryAsset* Archive);
	const FInventoryArchiveSlot* FindArchive(const UInventoryAsset* Archive) const { return FindArchive(Archive); }
};