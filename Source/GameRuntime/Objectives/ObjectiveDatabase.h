// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "DataTypes/CachedGetter.h"
#include "DataAssets/ToroDatabase.h"
#include "ObjectiveDatabase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Objective);

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Objective Entry")
struct FObjectiveDbEntry final : public FToroDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Objective)
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Objective, meta = (MultiLine = true))
		FText ShortText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Objective, meta = (MultiLine = true))
		FText DetailedText;

	virtual bool IsValid() const override
	{
		return !Label.IsEmptyOrWhitespace() 
			&& !ShortText.IsEmptyOrWhitespace()
			&& !DetailedText.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class UObjectiveDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UObjectiveDatabase()
		: Super(TAG_Objective.GetTag(), FObjectiveDbEntry::StaticStruct())
	{}

	UFUNCTION(BlueprintPure, Category = Database)
		static FObjectiveDbEntry GetObjectiveData(const FGameplayTag& Key);

	static const FObjectiveDbEntry* GetEntry(const FGameplayTag& Key);

private:

	static inline TCachedGetter<UObjectiveDatabase> Database{[]()
	{
		return UToroSettings::Get()->GetDatabase<UObjectiveDatabase>();
	}};
};
