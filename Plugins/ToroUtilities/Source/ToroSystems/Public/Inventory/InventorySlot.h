// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "JsonObjectWrapper.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROSYSTEMS_API FInvItemSlot
{
	GENERATED_BODY()

	UPROPERTY() FString Json;
	UPROPERTY() uint8 Amount;

	FInvItemSlot(): Json(TEXT("")), Amount(1) {}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInvItemSlot& Data)
	{
		Ar << Data.Json;
		Ar << Data.Amount;
		return Ar;
	}

	void SetJson(const FString& InJson)
	{
		FJsonObjectWrapper Wrapper;
		Wrapper.JsonObjectFromString(InJson);
		Wrapper.JsonObjectToString(Json);
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROSYSTEMS_API FInvArchiveSlot
{
	GENERATED_BODY()

	UPROPERTY() bool bSeen;
	UPROPERTY() bool bKnowSecret;
	UPROPERTY() int32 SortPriority;

	FInvArchiveSlot(): bSeen(false), bKnowSecret(false), SortPriority(0) {}
	FInvArchiveSlot(const bool bSecretFound, const int32 InSortPriority)
		: bSeen(false), bKnowSecret(bSecretFound), SortPriority(InSortPriority)
	{}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInvArchiveSlot& Data)
	{
		Ar << Data.bSeen;
		Ar << Data.bKnowSecret;
		Ar << Data.SortPriority;
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