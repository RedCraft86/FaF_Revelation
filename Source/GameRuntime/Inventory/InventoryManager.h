// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "DataTypes/PlayerTypes.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UInventoryManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryManager();

	UFUNCTION(BlueprintPure, Category = Inventory, meta = (WorldContext = ContextObject, DisplayName = "Get Inventory Manager"))
		static UInventoryManager* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool AddItem(UPARAM(meta=(Categories="Inventory.Item")) const FGameplayTag& Item, const bool bSort = true);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool RemoveItem(UPARAM(meta=(Categories="Inventory.Item")) const FGameplayTag& Item);

	UFUNCTION(BlueprintPure, Category = Inventory)
		bool HasItem(UPARAM(meta=(Categories="Inventory.Item")) const FGameplayTag& Item) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool AddArchive(UPARAM(meta=(Categories="Inventory.Archive")) const FGameplayTag& Archive, const bool bOpen = true);

	UFUNCTION(BlueprintPure, Category = Inventory)
		bool HasArchive(UPARAM(meta=(Categories="Inventory.Archive")) const FGameplayTag& Archive) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SortItems() { Items.Sort(); }

	UFUNCTION(BlueprintPure, Category = Inventory)
		const TArray<FGameplayTag>& GetItems() const { return Items; }

	UFUNCTION(BlueprintPure, Category = Inventory)
		const TArray<FGameplayTag>& GetArchives() const { return Archives; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void EquipItem(UPARAM(meta=(Categories="Inventory.Item")) const FGameplayTag& Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void UnequipItem();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Inventory)
		void UseEquipment() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool HasEquipment() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		const FGameplayTag& GetEquipment() const { return Equipment; }

	bool IsOpeningArchive() const { return OpeningArchive.IsSet(); }
	FGameplayTag GetOpeningArchive();

	void ClearItems() { Items.Empty(); }
	void EnsureEntries(const TArray<FGameplayTag>& Entries);

	TArray<FName> ExportArchives() const;
	void ImportArchives(const TArray<FName>& Keys);
	void SetEquipment(const FName InEquipment);

private:

	UPROPERTY(Transient)
		TArray<FGameplayTag> Items;

	UPROPERTY(Transient)
		TArray<FGameplayTag> Archives;

	UPROPERTY(Transient)
		FGameplayTag Equipment;

	UPROPERTY(Transient)
		TObjectPtr<class AEquipmentActor> EquipmentActor;

	UPROPERTY(Transient)
		TOptional<FGameplayTag> OpeningArchive;

	void SpawnEquipment(const FGameplayTag& InEquipment);
};
