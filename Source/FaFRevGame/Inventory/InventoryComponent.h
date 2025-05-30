// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryItemData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(Transient) TSoftObjectPtr<UInventoryItemData> Equipment;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryItemData>, int32> Items;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryItemData>, FInventoryArchive> Archives;

	FInventoryData() {}
	FInventoryData(const TSoftObjectPtr<UInventoryItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UInventoryItemData>, int32>& InItems,
		const TMap<TSoftObjectPtr<UInventoryItemData>, FInventoryArchive>& InArchives)
			: Equipment(InEquipment), Items(InItems), Archives(InArchives)
	{}

	friend FArchive& operator<<(FArchive& Ar, FInventoryData& Data)
	{
		Ar << Data.Items;
		Ar << Data.Archives;
		Ar << Data.Equipment;
		return Ar;
	}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

	uint8 AddItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 Amount = 1);
	uint8 RemoveItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 Amount = 1);
	void AddArchive(const TSoftObjectPtr<UInventoryItemData>& InArchive, const bool bShowHidden);

	uint8 GetItemAmount(const TSoftObjectPtr<UInventoryItemData>& InItem) const;
	bool HasItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 MinAmount = 1) const;
	
	void UnEquipItem();
	void EquipItem(const TSoftObjectPtr<UInventoryItemData>& InItem);

	TArray<TSoftObjectPtr<UInventoryItemData>> GetSortedItems();
	TArray<TSoftObjectPtr<UInventoryItemData>> GetSortedArchives();

	void LoadData(const FInventoryData& InData);
	FInventoryData SaveData() const { return InvData; }

private:

	UPROPERTY(Transient) FInventoryData InvData;
	UPROPERTY(Transient) TObjectPtr<AInventoryEquipment> EquipActor;
};
