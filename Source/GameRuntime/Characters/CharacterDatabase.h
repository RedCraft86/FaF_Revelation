// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "DataTypes/CachedGetter.h"
#include "DataAssets/ToroDatabase.h"
#include "CharacterDatabase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character);

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Character Entry")
struct FCharacterDbEntry final : public FToroDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, meta = (MultiLine = true))
		FText HelpText;

	virtual bool IsValid() const override
	{
		return !DisplayName.IsEmptyOrWhitespace() 
			&& !Description.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class UCharacterDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UCharacterDatabase()
		: Super(TAG_Character.GetTag(), FCharacterDbEntry::StaticStruct())
	{}

	UFUNCTION(BlueprintPure, Category = Database)
		static FCharacterDbEntry GetCharacterData(const FGameplayTag& Key);

	static const FCharacterDbEntry* GetEntry(const FGameplayTag& Key);

private:

	static inline TCachedGetter<UCharacterDatabase> Database{[]()
	{
		return UToroSettings::Get()->GetDatabase<UCharacterDatabase>();
	}};
};
