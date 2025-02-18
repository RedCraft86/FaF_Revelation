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
		: FilterMode(InMode), bCompareValues(InCompareValues), Metadata(InMetadata)
	{}

	bool Filter(const struct FInventorySlotData& InSlot) const;
	bool Filter(const UInventoryItemData* InItem, const FInventorySlotData& InSlot) const;
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
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = SaveData)
		FGameCurrency CurrencyData;
	
	UPROPERTY(EditAnywhere, Category = SaveData)
		TMap<FGuid, FInventorySlotData> ItemSlots;

	FInventorySaveData() {}
	friend FArchive& operator<<(FArchive& Ar, FInventorySaveData& SaveData)
	{
		Ar << SaveData.CurrencyData;
		Ar << SaveData.ItemSlots;
		return Ar;
	}

	bool IsValidData() const { return CurrencyData.GetAmount() != 0 || !ItemSlots.IsEmpty(); }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryIOUpdateSignature, const UInventoryItemData*, Item, const uint8, Count);

#define ON_UPDATE() { OnInventoryUpdate(); OnUpdate.Broadcast(); OnUpdateBP.Broadcast(); }
#define ON_ITEM_ADDED(Item, Count) { OnItemAdded.Broadcast(Item, Count); OnItemAddedBP.Broadcast(Item, Count); }
#define ON_ITEM_REMOVED(Item, Count) { OnItemRemoved.Broadcast(Item, Count); OnItemRemovedBP.Broadcast(Item, Count); }

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInventoryComponent();
	static UInventoryComponent* Get(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = InventoryManager)
		virtual const TMap<FGuid, FInventorySlotData>& GetConstInventory() const { return ItemSlots; }
	
	UFUNCTION(BlueprintPure, Category = InventoryManager)
		virtual bool IsValidSlot(const FGuid& InSlot) const { return InSlot.IsValid() && ItemSlots.Contains(InSlot); }
	
	UFUNCTION(BlueprintPure, Category = InventoryManager)
		virtual TArray<FGuid> GetSortedSlots() const;
	
	UFUNCTION(BlueprintPure, Category = InventoryManager)
		virtual TArray<FGuid> GetFilteredSlots(const TArray<FGuid>& InSlots, const TSet<EInventoryItemType>& TypeFilter, const bool bExcludeTypes = false) const;
	
	UFUNCTION(BlueprintPure, Category = InventoryManager, meta = (AdvancedDisplay = "Filter"))
		virtual FGuid FindSlot(const UInventoryItemData* Item, const FInventoryMetaFilter& Filter = FInventoryMetaFilter());

	UFUNCTION(BlueprintPure, Category = InventoryManager, meta = (AdvancedDisplay = "Filter"))
		virtual TSet<FGuid> FindSlots(const UInventoryItemData* Item, const FInventoryMetaFilter& Filter = FInventoryMetaFilter());
	
	UFUNCTION(BlueprintCallable, Category = InventoryManager)
		virtual void AppendSlotMetadata(const FGuid& InSlot, const FInventoryMetadata& InMetadata);

	UFUNCTION(BlueprintCallable, Category = InventoryManager)
		virtual void AddSlotMetadata(const FGuid& InSlot, const FGameplayTag InKey, const FString InValue);
	
	UFUNCTION(BlueprintCallable, Category = InventoryManager)
		virtual void RemoveSlotMetadata(const FGuid& InSlot, const FGameplayTag InKey);

	UFUNCTION(BlueprintCallable, Category = InventoryManager, meta = (AdvancedDisplay = "InValue"))
		virtual bool SlotHasMetadata(const FGuid& InSlot, const FGameplayTag InKey, const FString InValue = TEXT("*FilterAny"));
	
	UFUNCTION(BlueprintCallable, Category = InventoryManager, meta = (AdvancedDisplay = "bSilent"))
		virtual uint8 AddItemToSlot(const FGuid& InSlot, const uint8 Amount, const bool bSilent = false);
	
	UFUNCTION(BlueprintCallable, Category = InventoryManager, meta = (AdvancedDisplay = "bSilent"))
		virtual void AddInventoryItem(uint8& Overflow, TSet<FGuid>& Slots, UInventoryItemData* Item, const uint8 Amount, const FInventoryMetadata& Metadata, const bool bSilent = false);
	uint8 AddItem(UInventoryItemData* Item, const uint8 Amount, const FInventoryMetadata& Metadata, const bool bSilent = false);

	UFUNCTION(BlueprintCallable, Category = InventoryManager, meta = (AdvancedDisplay = "bSilent"))
		virtual uint8 RemoveItemFromSlot(const FGuid& InSlot, const uint8 Amount, const bool bSilent = false);
    
	UFUNCTION(BlueprintCallable, Category = InventoryManager, meta = (AdvancedDisplay = "Filter,bSilent"))
		virtual void RemoveInventoryItem(uint8& Missing, UInventoryItemData* Item, const uint8 Amount, const FInventoryMetaFilter& Filter = FInventoryMetaFilter(), const bool bSilent = false);
	uint8 RemoveItem(UInventoryItemData* Item, const uint8 Amount, const FInventoryMetaFilter& Filter = {}, const bool bSilent = false);

	UFUNCTION(BlueprintCallable, Category = InventoryManager)
		void EnsureItems(const TArray<FInventorySlotData>& InItems);

	UFUNCTION(BlueprintCallable, Category = InventoryManager)
		FInventorySaveData GetSaveData();

	UFUNCTION(BlueprintCallable, Category = InventoryManager)
		void SetSaveData(const FInventorySaveData& InData);

	DECLARE_MULTICAST_DELEGATE(FInventoryUpdateEvent);
	FInventoryUpdateEvent OnUpdate;
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryIOUpdateEvent, const UInventoryItemData*, const uint8);
	FInventoryIOUpdateEvent OnItemAdded, OnItemRemoved;
	
protected:

	UPROPERTY(BlueprintAssignable, DisplayName = "On Update")
		FInventoryUpdateSignature OnUpdateBP;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "On Item Added")
		FInventoryIOUpdateSignature OnItemAddedBP;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "On Item Removed")
		FInventoryIOUpdateSignature OnItemRemovedBP;
	
	UPROPERTY() FGameCurrency CurrencyData;
	UPROPERTY() TMap<FGuid, FInventorySlotData> ItemSlots;
	
	void ValidateInventory(const bool bForceUpdate = false);
	virtual void OnInventoryUpdate() {}
};
