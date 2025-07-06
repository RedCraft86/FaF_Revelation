// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EquipmentActor.h"
#include "MiscObjects/ToroDataAsset.h"
#include "InventoryAsset.generated.h"

UENUM(BlueprintType)
enum class EInvAssetType : uint8
{
	Item,
	Archive
};

UENUM(BlueprintType)
enum class EInvItemType : uint8
{
	Important,
	Generic,
	KeyItem,
	Equipment
};

UCLASS(NotBlueprintable, BlueprintType)
class TOROSYSTEMS_API UInventoryAsset final : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UInventoryAsset(): Type(EInvAssetType::Item), Priority(1), ItemType(EInvItemType::KeyItem), bReusableKey(true) {}

	UPROPERTY(EditAnywhere, Category = Asset)
		EInvAssetType Type;

	UPROPERTY(EditAnywhere, Category = Asset)
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = Asset)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Asset, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "Type == EInvAssetType::Item", EditConditionHides, ClampMax = 49, UIMax = 49))
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "Type == EInvAssetType::Item", EditConditionHides))
		EInvItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "Type == EInvAssetType::Item && ItemType == EInvItemType::KeyItem", EditConditionHides))
		bool bReusableKey;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "Type == EInvAssetType::Item && ItemType == EInvItemType::Equipment", EditConditionHides))
		TSoftClassPtr<AEquipmentActor> Equipment;

	UPROPERTY(EditAnywhere, Category = Archive, meta = (EditCondition = "Type == EInvAssetType::Archive", EditConditionHides, MultiLine = true))
		FText SecretText;

	UPROPERTY(EditAnywhere, Category = Archive, meta = (EditCondition = "Type == EInvAssetType::Archive", EditConditionHides, MultiLine = true))
		FText ContentText;

	uint8 GetStackLimit() const
	{
		return Type == EInvAssetType::Item && ItemType == EInvItemType::Generic ? 99 : 1;
	}
	
	uint8 GetPriority() const
	{
		if (Type == EInvAssetType::Archive) return 0;
		switch (ItemType)
		{
			case EInvItemType::Important: return Priority;
			case EInvItemType::Equipment: return Priority + 50;
			case EInvItemType::KeyItem: return Priority + 100;
			default: return Priority + 150;
		}
	}

	bool IsValidEquipment() const
	{
		return ItemType == EInvItemType::Equipment && !Equipment.IsNull();
	}

	bool IsValidArchive() const
	{
		return Type == EInvAssetType::Archive && !ContentText.IsEmptyOrWhitespace();
	}

#if WITH_EDITOR
	virtual FText GetDescription() const override
	{
		if (const UEnum* ItemTypeEnum = StaticEnum<EInvItemType>())
		{
			return Type == EInvAssetType::Archive ? INVTEXT("Archive")
				: ItemTypeEnum->GetDisplayNameTextByIndex((int32)ItemType);
		}
		return Super::GetDescription();
	}
#endif
};
