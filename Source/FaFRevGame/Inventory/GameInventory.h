// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputActionValue.h"
#include "InventoryItemData.h"
#include "Components/ActorComponent.h"
#include "GameInventory.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(Transient) TSoftObjectPtr<UInventoryItemData> Equipment;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryItemData>, uint8> Items;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryItemData>, uint8> Archives;

	FInventoryData() {}
	FInventoryData(const TSoftObjectPtr<UInventoryItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UInventoryItemData>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UInventoryItemData>, uint8>& InArchives)
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
class FAFREVGAME_API UGameInventory final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGameInventory();

	static UGameInventory* Get(const UObject* ContextObject);
	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UGameInventory* GetInventory(const UObject* ContextObject) { return Get(ContextObject); }

	uint8 AddItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 Amount = 1);
	uint8 RemoveItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 Amount = 1);
	void AddArchive(const TSoftObjectPtr<UInventoryItemData>& InArchive);

	uint8 GetItemAmount(const TSoftObjectPtr<UInventoryItemData>& InItem) const;
	bool HasItem(const TSoftObjectPtr<UInventoryItemData>& InItem, const uint8 MinAmount = 1) const;
	
	void UnEquipItem();
	void EquipItem(const TSoftObjectPtr<UInventoryItemData>& InItem);

	TArray<TSoftObjectPtr<UInventoryItemData>> GetSortedItems();
	TArray<TSoftObjectPtr<UInventoryItemData>> GetSortedArchives();

	void LoadSaveData(const FInventoryData& InData);
	FInventoryData GetSaveData() const { return InvData; }

	// Only used internally when the player SHOULD have a certain item/archive entry when changing game phases
	void EnsureInventory(const TSoftObjectPtr<UInventoryItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UInventoryItemData>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UInventoryItemData>, bool>& InArchives);

	void OpenInventory();
	void CloseInventory();
	void OpenToArchive(const UInventoryItemData* Archive);
	void EquipmentUse() const;

private:

	UPROPERTY() bool bInInventory;
	UPROPERTY(Transient) FInventoryData InvData;
	UPROPERTY(Transient) TObjectPtr<AInventoryEquipment> EquipActor;
};
