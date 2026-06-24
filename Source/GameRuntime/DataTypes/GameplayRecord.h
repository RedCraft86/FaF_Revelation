// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "GameplayRecord.generated.h"

USTRUCT(BlueprintType)
struct FGameplayRecord final
{
	GENERATED_BODY()

private:

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Category = Record, meta = (AllowPrivateAccess = true))
		int32 PlaySeconds;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Category = Record, meta = (AllowPrivateAccess = true))
		FDateTime Completion;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Category = Record, meta = (AllowPrivateAccess = true))
		TMap<FName, int32> Deaths;

public:

	FGameplayRecord()
		: PlaySeconds(0), Completion(0), Deaths({})
	{}

	FORCEINLINE bool IsCompleted() const { return Completion.GetTicks() != 0; }

	void IncrementPlayTime()
	{
		if (!IsCompleted())
		{
			PlaySeconds++;
		}
	}

	void MarkCompletion()
	{
		if (!IsCompleted())
		{
			Completion = FDateTime::Now();
		}
	}

	void AddDeath(const FGameplayTag& Character)
	{
		if (!IsCompleted())
		{
			Deaths.FindOrAdd(Character.GetTagName())++;
		}
	}

	// Track only completion since that is the only major identifier for duplicates of this kind of data
	FORCEINLINE bool operator==(const FGameplayRecord& Other) const { return Completion == Other.Completion; }
	FORCEINLINE bool operator!=(const FGameplayRecord& Other) const { return Completion != Other.Completion; }

	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, FGameplayRecord& InRecord)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << SA_VALUE(TEXT("PlaySeconds"), InRecord.PlaySeconds);
		Record << SA_VALUE(TEXT("Completion"), InRecord.Completion);
		Record << SA_VALUE(TEXT("Deaths"), InRecord.Deaths);
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGameplayRecord& InRecord)
	{
		Ar << InRecord.PlaySeconds;
		Ar << InRecord.Completion;
		Ar << InRecord.Deaths;
		return Ar;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FGameplayRecord& Record)
	{
		// Track only completion since that is the only major identifier for duplicates of this kind of data
		return GetTypeHash(Record.Completion);
	}
};