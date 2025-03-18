// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "JsonObjectWrapper.h"
#include "GameFramework/Actor.h"
#include "InventoryEquipment.generated.h"

struct FInvSlotData;
class UInventoryComponent;

UCLASS(Abstract)
class TOROUTILITIES_API AInventoryEquipment final : public AActor
{
	GENERATED_BODY()

public:

	AInventoryEquipment();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobject)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintImplementableEvent)
		void UseItem();

	UFUNCTION(BlueprintImplementableEvent)
		void BeginAltUse();

	UFUNCTION(BlueprintImplementableEvent)
		void EndAltUse();

	void OnEquip(UInventoryComponent* Inventory, const FInvSlotData& Slot);
	void OnUnquip(UInventoryComponent* Inventory, FInvSlotData& Slot);
	void SaveData(FInvSlotData& Slot) const;

protected:

	UPROPERTY() FJsonObjectWrapper CachedJson;

	UFUNCTION(BlueprintImplementableEvent)
		void EquipItem(UInventoryComponent* Inventory, const FJsonObjectWrapper& JsonObject);

	UFUNCTION(BlueprintImplementableEvent)
		void UnequipItem(UInventoryComponent* Inventory, FJsonObjectWrapper& JsonObject);
};
