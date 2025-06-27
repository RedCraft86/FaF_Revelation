// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryAsset.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROSYSTEMS_API FInvItemSlot
{
	GENERATED_BODY()

	UPROPERTY() FString Json;
	UPROPERTY() uint8 Amount;
	UPROPERTY(Transient) TSoftObjectPtr<UInventoryAsset> Asset;

	FInvItemSlot(): Json(TEXT("")), Amount(0) {}
	FInvItemSlot(const TSoftObjectPtr<UInventoryAsset>& InAsset): Json(TEXT("")), Amount(1), Asset(InAsset) {}
	FInvItemSlot(const TSoftObjectPtr<UInventoryAsset>& InAsset, const uint8 InAmount = 1, const FString& InJson = TEXT(""))
		: Json(InJson), Amount(InAmount), Asset(InAsset)
	{}

	FORCEINLINE bool operator==(const FInvItemSlot& Other) const { return Asset == Other.Asset; }
	FORCEINLINE bool operator!=(const FInvItemSlot& Other) const { return !(*this == Other); }
	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInvItemSlot& Data)
	{
		Ar << Data.Json;
		Ar << Data.Amount;
		Ar << Data.Asset;
		return Ar;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROSYSTEMS_API FInvArchiveSlot
{
	GENERATED_BODY()

	UPROPERTY() bool bSeen;
	UPROPERTY() bool bKnowSecret;
	UPROPERTY(Transient) TSoftObjectPtr<UInventoryAsset> Asset;

	FInvArchiveSlot(): bSeen(false), bKnowSecret(false) {}
	FInvArchiveSlot(const TSoftObjectPtr<UInventoryAsset>& InAsset, const bool bSecretFound = false)
		: bSeen(false), bKnowSecret(bSecretFound), Asset(InAsset)
	{}

	FORCEINLINE bool operator==(const FInvArchiveSlot& Other) const { return Asset == Other.Asset; }
	FORCEINLINE bool operator!=(const FInvArchiveSlot& Other) const { return !(*this == Other); }
	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInvArchiveSlot& Data)
	{
		Ar << Data.bSeen;
		Ar << Data.bKnowSecret;
		Ar << Data.Asset;
		return Ar;
	}

	void FoundSecret()
	{
		if (!bKnowSecret)
		{
			bSeen = false;
			bKnowSecret = true;
		}
	}
};