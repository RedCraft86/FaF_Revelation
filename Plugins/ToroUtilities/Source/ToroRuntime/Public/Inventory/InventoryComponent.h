// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItemData;

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInventorySlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = SlotData)
		TSoftObjectPtr<UInventoryItemData> Item;

	UPROPERTY(EditAnywhere, Category = SlotData)
		uint8 Amount;

	UPROPERTY(EditAnywhere, Category = SlotData, meta = (MultiLine = true, ForceInlineRow, Categories = "InventoryMeta"))
		TMap<FGameplayTag, FString> Metadata;

	FInventorySlotData() : Amount(0) {}
    
	friend FArchive& operator<<(FArchive& Ar, FInventorySlotData& SlotData)
	{
		Ar << SlotData.Item;
		Ar << SlotData.Amount;
		Ar << SlotData.Metadata;
		return Ar;
	}

	bool IsValidData() const { return !Item.IsNull() && Amount > 0; }
	TMap<FGameplayTag, FString> GetMetadata() const;
	UInventoryItemData* GetItem() const;
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInventoryComponent();

protected:

	UPROPERTY() TArray<FInventorySlotData> Slots;
};
