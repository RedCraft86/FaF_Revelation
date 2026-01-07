// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Actors/ToroCharacter.h"
#include "MiscObjects/ToroDataAsset.h"
#include "GameOverDatabase.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FAFREVGAME_API FGameOverCharEntry : public FDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Entry, meta = (MultiLine = true))
		FText Name;

	UPROPERTY(EditAnywhere, Category = Entry, meta = (MultiLine = true))
		FText Description;

	virtual bool IsValidData() const override
	{
		return !Name.IsEmptyOrWhitespace()
			&& !Description.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class FAFREVGAME_API UGameOverDatabase : public UToroDatabase
{
	GENERATED_BODY()

public:

	UGameOverDatabase() {}

	UPROPERTY(EditAnywhere, Category = Database, meta = (ForceInlineRow, Categories = "Character", TitleProperty = "Label"))
		TMap<FGameplayTag, FGameOverCharEntry> Entries;

	const FGameOverCharEntry* GetEntry(const FGameplayTag& Key) const
	{
		return CharacterTags::IsValidTag(Key) && Key != CharacterTags::TAG_Player ? Entries.Find(Key) : nullptr;
	}

#if WITH_EDITOR
	virtual FIntPoint GetDataResults() const override
	{
		int32 Valid = 0;
		for (const TPair<FGameplayTag, FGameOverCharEntry>& Entry : Entries)
		{
			if (CharacterTags::IsValidTag(Entry.Key) 
				&& Entry.Key != CharacterTags::TAG_Player
				&& Entry.Value.IsValidData())
			{
				Valid++;
			}
		}
		return FIntPoint(Entries.Num(), Valid);
	}
protected:
	virtual void UpdateData() override
	{
		for (TPair<FGameplayTag, FGameOverCharEntry>& Entry : Entries)
		{
			if (!CharacterTags::IsValidTag(Entry.Key) || 
				Entry.Key == CharacterTags::TAG_Player)
			{
				Entry.Value.Label = TEXT("ERROR: Invalid Tag/Key");
			}
			else if (!Entry.Value.IsValidData())
			{
				Entry.Value.Label = TEXT("ERROR: Invalid Data/Entry");
			}
			else
			{
				Entry.Value.Label = Entry.Value.Name.ToString();
			}
		}
	}
#endif
};
