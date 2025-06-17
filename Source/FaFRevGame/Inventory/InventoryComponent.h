// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryItemData.h"
#include "DataTypes/InputModeData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(Transient) TSoftObjectPtr<UItemData> Equipment;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UItemData>, uint8> Items;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UItemData>, FInvArchiveData> Archives;

	FInventoryData() {}
	FInventoryData(const TSoftObjectPtr<UItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UItemData>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UItemData>, FInvArchiveData>& InArchives)
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

	static UInventoryComponent* Get(const UObject* ContextObject);
	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UInventoryComponent* GetInventory(const UObject* ContextObject) { return Get(ContextObject); }

	uint8 AddItem(const TSoftObjectPtr<UItemData>& InItem, const uint8 Amount = 1);
	uint8 RemoveItem(const TSoftObjectPtr<UItemData>& InItem, const uint8 Amount = 1);

	void AddArchive(const TSoftObjectPtr<UItemData>& InArchive, const bool bSecretFound);
	void GetArchiveState(const TSoftObjectPtr<UItemData>& InArchive, bool& bDiscovered, bool& bSecretFound) const;

	uint8 GetItemAmount(const TSoftObjectPtr<UItemData>& InItem) const;
	bool HasItem(const TSoftObjectPtr<UItemData>& InItem, const uint8 MinAmount = 1) const;
	
	void UnEquipItem();
	void EquipItem(const TSoftObjectPtr<UItemData>& InItem);
	const TSoftObjectPtr<UItemData>& GetEquipment() { return InvData.Equipment; }

	TArray<TSoftObjectPtr<UItemData>> GetSortedItems();
	TArray<TSoftObjectPtr<UItemData>> GetSortedArchives();

	void LoadSaveData(const FInventoryData& InData);
	FInventoryData GetSaveData() const { return InvData; }

	// Only used internally when the player SHOULD have a certain item/archive entry when changing game phases
	void EnsureInventory(const TSoftObjectPtr<UItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UItemData>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UItemData>, bool>& InArchives);

	void OpenInventory();
	void CloseInventory();
	void SetEquipmentUse(const bool bUsing) const;

private:

	UPROPERTY() bool bInInventory;
	UPROPERTY(Transient) FInventoryData InvData;
	UPROPERTY(Transient) FGameInputModeData InputModeCache;
	UPROPERTY(Transient) TObjectPtr<class AGamePlayerChar> Player;
	UPROPERTY(Transient) TObjectPtr<AInventoryEquipment> EquipActor;
	// UPROPERTY(Transient) TObjectPtr<UInventoryWidget> Widget;

	// UInventoryWidget* GetWidget(); TODO
	virtual void BeginPlay() override;
};
