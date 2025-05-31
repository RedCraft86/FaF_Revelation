// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "InventoryEquipment.h"
#include "InventoryItemData.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	Generic,
	Equipment,
	Archive
};

UCLASS(NotBlueprintable, BlueprintType)
class FAFREVGAME_API UInventoryItemData final : public UDataAsset
{
	GENERATED_BODY()

public:

	UInventoryItemData(): Priority(1), ItemType(EInventoryItemType::Generic), StackSize(1) {}

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType != EInventoryItemType::Archive", InlineEditConditionToggle))
		bool bCustomPriority = false;
#endif

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "bCustomPriority && ItemType != EInventoryItemType::Archive"))
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = Item)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Item)
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = Item, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = Item)
		EInventoryItemType ItemType;

	UPROPERTY(EditAnywhere, Category = Item, meta = (ClampMin = 1, UIMin = 1, ClampMax = 10, UIMax = 10, EditCondition = "ItemType == EInventoryItemType::Generic"))
		uint8 StackSize;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType == EInventoryItemType::Equipment", EditConditionHides))
		TSoftClassPtr<AInventoryEquipment> Equipment;

	UPROPERTY(EditAnywhere, Category = Item, meta = (EditCondition = "ItemType == EInventoryItemType::Archive", EditConditionHides))
		FText HiddenText;
	
#if WITH_EDITOR
private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
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

USTRUCT(BlueprintInternalUseOnly)
struct FInventoryArchive
{
	GENERATED_BODY()

	UPROPERTY() int32 ArchiveIdx;
	UPROPERTY() bool bShowHidden;

	FInventoryArchive(): ArchiveIdx(0), bShowHidden(false) {}
	FInventoryArchive(const int32 InArchiveIdx, const bool bInShowHidden)
		: ArchiveIdx(FMath::Max(0, InArchiveIdx)), bShowHidden(bInShowHidden)
	{}

	friend FArchive& operator<<(FArchive& Ar, FInventoryArchive& Data)
	{
		Ar << Data.ArchiveIdx;
		Ar << Data.bShowHidden;
		return Ar;
	}
};