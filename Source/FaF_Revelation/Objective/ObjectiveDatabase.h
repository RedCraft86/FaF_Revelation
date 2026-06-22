// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "FaFRevSettings.h"
#include "GameplayTagContainer.h"
#include "DataAssets/ToroDatabase.h"
#include "ObjectiveDatabase.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveEntry : public FDatabaseEntryBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective, meta = (MultiLine = true))
		FText ShortDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective, meta = (MultiLine = true))
		FText DetailDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		TSoftObjectPtr<AObjectiveActor> Instance;
	
	FObjectiveEntry()
		: Name(FText::GetEmpty()), ShortDesc(FText::GetEmpty())
		, DetailDesc(FText::GetEmpty()), Instance(nullptr)
	{}

	FORCEINLINE bool IsValid() const
	{
		return Name.IsEmptyOrWhitespace() || ShortDesc.IsEmptyOrWhitespace() || DetailDesc.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class UObjectiveDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:
	
	UObjectiveDatabase()
	{
		DisplayName = INVTEXT("Objective Database");
		Description = INVTEXT("Holds static metadata for objectives.");
	}

	UPROPERTY(EditAnywhere, Category = Database, meta = (Categories = "Objective"))
		TMap<FGameplayTag, FObjectiveEntry> Entries;

	static const FObjectiveEntry* GetEntry(const FGameplayTag& InTag)
	{
		UObjectiveDatabase* DB = UFaFRevSettings::Get()->GetDatabase<UObjectiveDatabase>();
		return DB ? DB->Entries.Find(InTag) : nullptr;
	}

#if WITH_EDITOR
	virtual void RefreshData() override
	{
		for (TPair<FGameplayTag, FObjectiveEntry>& Entry : Entries)
		{
			if (!Entry.Key.IsValid())
			{
				Entry.Value.EdLabel = TEXT("Key is invalid.");
			}
			else if (Entry.Value.IsValid())
			{
				Entry.Value.EdLabel = TEXT("Data is invalid.");
			}
			else
			{
				Entry.Value.EdLabel = FString();
			}
		}
	}

	virtual FIntPoint GetEntryCount() const override
	{
		FIntPoint Result(Entries.Num(), 0);
		for (const TPair<FGameplayTag, FObjectiveEntry>& Entry : Entries)
		{
			if (Entry.Value.EdLabel.IsEmpty())
			{
				Result.Y++;
			}
		}
		return Result;
	}
#endif
};
