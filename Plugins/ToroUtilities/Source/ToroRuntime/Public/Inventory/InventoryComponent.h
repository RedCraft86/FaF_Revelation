// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MiscTypes.h"
#include "InventoryItemData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItemData;

UENUM(BlueprintType)
enum class EInventoryMetaFilterMode : uint8
{
	Unfiltered,
	MatchAny,
	MatchAll
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInventoryMetaFilter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetaFilter)
		EInventoryMetaFilterMode FilterMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetaFilter)
		bool bCompareValues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetaFilter)
		TMap<FGameplayTag, FString> Metadata;

	FInventoryMetaFilter()
		: FilterMode(EInventoryMetaFilterMode::Unfiltered), bCompareValues (false), Metadata({})
	{}
	
	FInventoryMetaFilter(const EInventoryMetaFilterMode InMode, const bool InCompareValues, const TMap<FGameplayTag, FString>& InMetadata)
		: FilterMode(InMode), bCompareValues (InCompareValues), Metadata(InMetadata)
	{}

	bool Filter(const struct FInventorySlotData& InSlot) const;
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInventorySlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SlotData)
		TSoftObjectPtr<UInventoryItemData> Item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SlotData)
		uint8 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SlotData)
		FInventoryMetadata Metadata;

	FInventorySlotData() : Amount(0) {}
	explicit FInventorySlotData(const TObjectPtr<UInventoryItemData>& InItem, const uint8 InAmount = 1, const FInventoryMetadata& InMetadata = {});
	friend FArchive& operator<<(FArchive& Ar, FInventorySlotData& SlotData)
	{
		Ar << SlotData.Item;
		Ar << SlotData.Amount;
		Ar << SlotData.Metadata;
		return Ar;
	}

	bool IsValidData() const { return !Item.IsNull() && Amount > 0; }
	FInventoryMetadata GetMetadata() const;
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
