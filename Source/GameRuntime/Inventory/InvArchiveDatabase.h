// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Engine/Texture2D.h"
#include "DataTypes/CachedGetter.h"
#include "DataAssets/ToroDatabase.h"
#include "InvArchiveDatabase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InventoryArchive)

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Inventory Archive Entry")
struct FInvArchiveDbEntry final : public FToroDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Archive)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Archive, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Archive)
		TSoftObjectPtr<UTexture2D> IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Archive, meta = (MultiLine = true))
		FText ContentText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Archive)
		TSoftObjectPtr<UTexture2D> ContentImage;

	virtual bool IsValid() const override
	{
		return !DisplayName.IsEmptyOrWhitespace() 
			&& !Description.IsEmptyOrWhitespace()
			&& !IconImage.IsNull() 
			&& !(ContentText.IsEmptyOrWhitespace() && ContentImage.IsNull());
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class UInvArchiveDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UInvArchiveDatabase()
		: Super(TAG_InventoryArchive.GetTag(), FInvArchiveDbEntry::StaticStruct())
	{}

	UFUNCTION(BlueprintPure, Category = Database)
		static FInvArchiveDbEntry GetArchiveData(const FGameplayTag& Key);

	static const FInvArchiveDbEntry* GetEntry(const FGameplayTag& Key);

private:

	static inline TCachedGetter<UInvArchiveDatabase> Database{[]()
	{
		return UToroSettings::Get()->GetDatabase<UInvArchiveDatabase>();
	}};
};
