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
	KeyItem,
	QuestItem,
	Equipment
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UInventoryAsset final : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UInventoryAsset();

	UPROPERTY(EditAnywhere, Category = Asset)
		EInvAssetType AssetType;

	UPROPERTY(EditAnywhere, Category = Asset)
		TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, Category = Asset)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Asset, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "AssetType == EInvAssetType::Item", EditConditionHides))
		EInvItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "AssetType == EInvAssetType::Item && ItemType == EInvItemType::KeyItem", EditConditionHides))
		bool bReusableKey;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "AssetType == EInvAssetType::Item && ItemType == EInvItemType::Equipment", EditConditionHides))
		TSoftClassPtr<AEquipmentActor> Equipment;

	UPROPERTY(EditAnywhere, Category = Archive, meta = (EditCondition = "AssetType == EInvAssetType::Archive", EditConditionHides, MultiLine = true))
		FText ContentText;

	uint8 GetStackLimit() const;
	bool IsValidEquipment() const;
	bool IsValidArchive() const;

#if WITH_EDITOR
	virtual FText GetDescription() const override;
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryItems
{
	GENERATED_BODY()

	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, uint8> ItemMap;

	FInventoryItems(): ItemMap({}) {}
	FInventoryItems(const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems)
		: ItemMap(InItems)
	{}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryItems& Data)
	{
		Ar << Data.ItemMap;
		return Ar;
	}
};