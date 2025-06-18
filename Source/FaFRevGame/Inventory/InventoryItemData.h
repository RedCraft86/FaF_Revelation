// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryEquipment.h"
#include "MiscObjects/ToroDataAsset.h"
#include "InventoryItemData.generated.h"

UENUM(BlueprintType)
enum class EInvItemType : uint8
{
	Generic,
	Equipment,
	Archive
};

USTRUCT(BlueprintType)
struct FInvArchiveData
{
	GENERATED_BODY()

	UPROPERTY() uint8 EntryNum;
	UPROPERTY() bool bSecretFound;

	FInvArchiveData(): EntryNum(0), bSecretFound(false) {}
	FInvArchiveData(const uint8 InEntryNum, const bool bFoundSecret)
		: EntryNum(InEntryNum), bSecretFound(bFoundSecret)
	{}

	friend FArchive& operator<<(FArchive& Ar, FInvArchiveData& Data)
	{
		Ar << Data.EntryNum;
		Ar << Data.bSecretFound;
		return Ar;
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class FAFREVGAME_API UInvItemData final : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UInvItemData(): Priority(1), ItemType(EInvItemType::Generic), StackSize(1) {}

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType != EInvItemType::Archive"))
		bool bCustomPriority = false;
#endif

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "bCustomPriority && ItemType != EInvItemType::Archive"))
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = Item)
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = Item)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Item, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = Item)
		EInvItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Item, meta = (ClampMin = 1, UIMin = 1, ClampMax = 10, UIMax = 10, EditCondition = "ItemType == EInvItemType::Generic"))
		uint8 StackSize;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType == EInvItemType::Equipment", EditConditionHides))
		TSoftClassPtr<AInventoryEquipment> Equipment;

	UPROPERTY(EditAnywhere, Category = Item, meta = (MultiLine = true, EditCondition = "ItemType == EInvItemType::Archive", EditConditionHides))
		FText SecretLabel;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType == EInvItemType::Archive", EditConditionHides))
		FText SecretText;
	
#if WITH_EDITOR
private:
	virtual FText GetDescription() const override { return DisplayName; }
	virtual void UpdateData() override
	{
		if (ItemType != EInvItemType::Generic) StackSize = 1;
		if (!bCustomPriority)
		{
			switch (ItemType)
			{
			case EInvItemType::Generic: Priority = 5; break;
			case EInvItemType::Equipment: Priority = 1; break;
			case EInvItemType::Archive: Priority = 10; break;
			}
		}
	}
#endif
};