// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Engine/Texture2D.h"
#include "DataTypes/CachedGetter.h"
#include "DataAssets/ToroDatabase.h"
#include "InvItemDatabase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InventoryItem)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InvEquipment)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InvMission)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InvKey)

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Inventory Item Entry")
struct FInvItemDbEntry final : public FToroDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		TSoftObjectPtr<UTexture2D> IconImage;

	virtual bool IsValid() const override
	{
		return !DisplayName.IsEmptyOrWhitespace() 
			&& !Description.IsEmptyOrWhitespace()
			&& !IconImage.IsNull();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class UInvItemDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UInvItemDatabase()
		: Super(TAG_InventoryItem.GetTag(), FInvItemDbEntry::StaticStruct())
	{}

	UFUNCTION(BlueprintPure, Category = Database)
		static FInvItemDbEntry GetItemData(const FGameplayTag& Key);

	static const FInvItemDbEntry* GetEntry(const FGameplayTag& Key);

private:

	static inline TCachedGetter<UInvItemDatabase> Database{[]()
	{
		return UToroSettings::Get()->GetDatabase<UInvItemDatabase>();
	}};
};
