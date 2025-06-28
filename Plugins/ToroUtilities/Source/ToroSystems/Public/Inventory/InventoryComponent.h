// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventorySlot.h"
#include "EquipmentActor.h"
#include "InventoryAsset.h"
#include "InventoryWidget.h"
#include "DataTypes/InputModeData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct TOROSYSTEMS_API FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(Transient) TSoftObjectPtr<UInventoryAsset> Equipment;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, FInvItemSlot> Items;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, FInvArchiveSlot> Archives;

	FInventoryData() {}
	FInventoryData(const TSoftObjectPtr<UInventoryAsset>& InEquipment,
		const TMap<TSoftObjectPtr<UInventoryAsset>, FInvItemSlot>& InItems,
		const TMap<TSoftObjectPtr<UInventoryAsset>, FInvArchiveSlot>& InArchives)
			: Equipment(InEquipment), Items(InItems), Archives(InArchives)
	{}

	friend FArchive& operator<<(FArchive& Ar, FInventoryData& Data)
	{
		Ar << Data.Items;
		Ar << Data.Archives;
		Ar << Data.Equipment;
		return Ar;
	}
	
	void EnsureData(const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UInventoryAsset>, bool>& InArchives);
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UInventoryComponent* GetInventoryComponent(const UObject* ContextObject, const int32 PlayerIndex);

	static UInventoryComponent* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return GetInventoryComponent(ContextObject, PlayerIndex);
	}

	UFUNCTION(BlueprintCallable, Category = Inventory)
		uint8 AddItem(const TSoftObjectPtr<UInventoryAsset>& InItem, const uint8 Amount = 1, const FString& Json = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = Inventory)
		uint8 RemoveItem(const TSoftObjectPtr<UInventoryAsset>& InItem, const uint8 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool UseKeyItem(const TSoftObjectPtr<UInventoryAsset>& InItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool GetItemSlot(const TSoftObjectPtr<UInventoryAsset>& InItem, FInvItemSlot& Slot) const;

	UFUNCTION(BlueprintPure, Category = Inventory)
		TArray<TSoftObjectPtr<UInventoryAsset>> GetSortedItems();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddArchive(const TSoftObjectPtr<UInventoryAsset>& InArchive, const bool bKnowSecret = false);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool GetArchiveSlot(const TSoftObjectPtr<UInventoryAsset>& InArchive, FInvArchiveSlot& Slot) const;

	UFUNCTION(BlueprintPure, Category = Inventory)
		TArray<TSoftObjectPtr<UInventoryAsset>> GetSortedArchives();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void UnEquipItem();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void EquipItem(const TSoftObjectPtr<UInventoryAsset>& InItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SetEquipmentUsage(const bool bUsing) const;

	UFUNCTION(BlueprintPure, Category = Inventory)
		const TSoftObjectPtr<UInventoryAsset>& GetEquipment() { return InvData.Equipment; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void CloseInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void LoadSaveData(const FInventoryData& InData);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		FInventoryData GetSaveData() const { return InvData; }

	// Only used internally when the player SHOULD have a certain item/archive entry when changing game phases
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void EnsureInventory(const TSoftObjectPtr<UInventoryAsset>& InEquipment,
			const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems,
			const TMap<TSoftObjectPtr<UInventoryAsset>, bool>& InArchives);

	DECLARE_DELEGATE_RetVal(USceneComponent*, FGetEquipmentRoot);
	FGetEquipmentRoot GetEquipmentRoot;

protected:

	UPROPERTY() bool bInInventory;
	UPROPERTY() FInventoryData InvData;
	UPROPERTY() FGameInputModeData CachedInput;
	UPROPERTY(Transient) TObjectPtr<AEquipmentActor> EquipActor;
	UPROPERTY(Transient) TObjectPtr<UInventoryWidget> WidgetPtr;

	UInventoryWidget* GetWidget();
};
