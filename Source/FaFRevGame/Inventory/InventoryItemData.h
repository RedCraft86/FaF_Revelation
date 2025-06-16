// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryEquipment.h"
#include "MiscObjects/ToroDataAsset.h"
#include "InventoryItemData.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
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
class FAFREVGAME_API UInventoryItemData final : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UInventoryItemData(): Priority(1), ItemType(EInventoryItemType::Generic), StackSize(1) {}

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType != EInventoryItemType::Archive"))
		bool bCustomPriority = false;
#endif

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "bCustomPriority && ItemType != EInventoryItemType::Archive"))
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = Item)
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = Item)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Item, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = Item)
		EInventoryItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Item, meta = (ClampMin = 1, UIMin = 1, ClampMax = 10, UIMax = 10, EditCondition = "ItemType == EInventoryItemType::Generic"))
		uint8 StackSize;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType == EInventoryItemType::Equipment", EditConditionHides))
		TSoftClassPtr<AInventoryEquipment> Equipment;

	UPROPERTY(EditAnywhere, Category = Item, meta = (MultiLine = true, EditCondition = "ItemType == EInventoryItemType::Archive", EditConditionHides))
		FText SecretLabel;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType == EInventoryItemType::Archive", EditConditionHides))
		FText SecretText;
	
#if WITH_EDITOR
private:
	virtual FText GetDescription() const override { return DisplayName; }
	virtual void UpdateData() override
	{
		if (ItemType != EInventoryItemType::Generic) StackSize = 1;
		if (!bCustomPriority)
		{
			switch (ItemType)
			{
			case EInventoryItemType::Generic: Priority = 5; break;
			case EInventoryItemType::Equipment: Priority = 1; break;
			case EInventoryItemType::Archive: Priority = 10; break;
			}
		}
	}
#endif
};