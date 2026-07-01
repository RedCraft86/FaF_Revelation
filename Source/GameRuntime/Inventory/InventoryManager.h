// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InventoryArchive)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InventoryItem)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InvEquipment)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InvMission)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InvKey)

// TODO: Item and Archive database

UCLASS(NotBlueprintable, BlueprintType)
class UInventoryManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryManager();

	UFUNCTION(BlueprintPure, Category = Inventory, meta = (WorldContext = ContextObject, DisplayName = "Get Inventory Manager"))
		static UInventoryManager* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddItem(const FGameplayTag& Item, const bool bSort = true);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void RemoveItem(const FGameplayTag& Item);

	UFUNCTION(BlueprintPure, Category = Inventory)
		bool HasItem(const FGameplayTag& Item) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddArchive(const FGameplayTag& Archive);

	UFUNCTION(BlueprintPure, Category = Inventory)
		bool HasArchive(const FGameplayTag& Archive) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void SortItems() { Items.Sort(); }

	UFUNCTION(BlueprintPure, Category = Inventory)
		const TArray<FGameplayTag>& GetItems() const { return Items; }

	UFUNCTION(BlueprintPure, Category = Inventory)
		const TArray<FGameplayTag>& GetArchives() const { return Archives; }

	void ClearItems() { Items.Empty(); }
	void EnsureEntries(const TArray<FGameplayTag>& Entries);

	TArray<FName> ExportArchives() const;
	void ImportArchives(const TArray<FName>& Keys);

private:

	UPROPERTY(Transient)
		TArray<FGameplayTag> Items;

	UPROPERTY(Transient)
		TArray<FGameplayTag> Archives;
};
