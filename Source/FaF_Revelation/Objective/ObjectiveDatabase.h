// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "DataAssets/ToroDatabase.h"
#include "ObjectiveDatabase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Objective)

USTRUCT(BlueprintType)
struct FObjectiveEntry : public FToroDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective, meta = (MultiLine = true))
		FText ShortDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective, meta = (MultiLine = true))
		FText DetailDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		TSoftObjectPtr<class AObjectiveActor> Instance;
	
	FObjectiveEntry()
		: Name(FText::GetEmpty()), ShortDesc(FText::GetEmpty())
		, DetailDesc(FText::GetEmpty()), Instance(nullptr)
	{}

	virtual bool IsValid() const override
	{
		return !Name.IsEmptyOrWhitespace() && !ShortDesc.IsEmptyOrWhitespace() && !DetailDesc.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class UObjectiveDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UObjectiveDatabase(const FObjectInitializer& ObjectInit)
		: Super(ObjectInit, TAG_Objective.GetTag(), FObjectiveEntry::StaticStruct())
	{}

	[[nodiscard]] static UObjectiveDatabase* Get();
	[[nodiscard]] static const FObjectiveEntry* GetEntry(const FGameplayTag& Key);
};
