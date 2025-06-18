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

	UPROPERTY(Transient) TSoftObjectPtr<UInvItemData> Equipment;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInvItemData>, uint8> Items;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInvItemData>, FInvArchiveData> Archives;

	FInventoryData() {}
	FInventoryData(const TSoftObjectPtr<UInvItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UInvItemData>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UInvItemData>, FInvArchiveData>& InArchives)
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

	uint8 AddItem(const TSoftObjectPtr<UInvItemData>& InItem, const uint8 Amount = 1);
	uint8 RemoveItem(const TSoftObjectPtr<UInvItemData>& InItem, const uint8 Amount = 1);

	void AddArchive(const TSoftObjectPtr<UInvItemData>& InArchive, const bool bSecretFound);
	void GetArchiveState(const TSoftObjectPtr<UInvItemData>& InArchive, bool& bDiscovered, bool& bSecretFound) const;

	uint8 GetItemAmount(const TSoftObjectPtr<UInvItemData>& InItem) const;
	bool HasItem(const TSoftObjectPtr<UInvItemData>& InItem, const uint8 MinAmount = 1) const;
	
	void UnEquipItem();
	void EquipItem(const TSoftObjectPtr<UInvItemData>& InItem);
	const TSoftObjectPtr<UInvItemData>& GetEquipment() { return InvData.Equipment; }

	TArray<TSoftObjectPtr<UInvItemData>> GetSortedItems();
	TArray<TSoftObjectPtr<UInvItemData>> GetSortedArchives();

	void LoadSaveData(const FInventoryData& InData);
	FInventoryData GetSaveData() const { return InvData; }

	// Only used internally when the player SHOULD have a certain item/archive entry when changing game phases
	void EnsureInventory(const TSoftObjectPtr<UInvItemData>& InEquipment,
		const TMap<TSoftObjectPtr<UInvItemData>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UInvItemData>, bool>& InArchives);

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
