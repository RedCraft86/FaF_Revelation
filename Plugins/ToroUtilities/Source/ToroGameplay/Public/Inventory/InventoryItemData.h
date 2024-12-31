// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "InventoryItemAttributes.h"
#include "StructUtils/InstancedStruct.h"
#include "InventoryItemData.generated.h"

#define ITEM_LIMIT_MAX 9999

UENUM(BlueprintType)
enum class EInventoryStackType : uint8
{
	UntilMax,
	Defined,
	Unique
};

UCLASS(NotBlueprintable, BlueprintType)
class TOROGAMEPLAY_API UInventoryItemData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = General)
		uint8 Priority;

	UPROPERTY(EditAnywhere, Category = General)
		FGuid UniqueID;

	UPROPERTY(EditAnywhere, Category = General)
		TObjectPtr<UTexture2D> Thumbnail;
	
	UPROPERTY(EditAnywhere, Category = General)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = General, meta = (MultiLine = true))
		FText Description;
	
	UPROPERTY(EditAnywhere, Category = General)
		EInventoryStackType StackingMode;
	
	UPROPERTY(EditAnywhere, Category = General, meta = (ClampMin = 2, UIMin = 2, EditCondition = "StackingMode == EInventoryStackType::Defined", EditConditionHides))
		uint8 MaxStackSize;

	UPROPERTY(EditAnywhere, Category = General, meta = (ClampMin = 1, UIMin = 1, EditCondition = "StackingMode == EInventoryStackType::Unique", EditConditionHides))
		uint8 MaxNumOfSlots;

	UPROPERTY(EditAnywhere, Category = Attributes, meta = (MultiLine = true))
		TMap<FName, FString> Metadata;

	UPROPERTY(EditAnywhere, Category = Attributes, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FInventoryItemAttribute>> Attributes;
};
