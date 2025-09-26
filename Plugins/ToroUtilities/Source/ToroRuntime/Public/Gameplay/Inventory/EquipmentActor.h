// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "JsonObjectWrapper.h"
#include "GameFramework/Actor.h"
#include "EquipmentActor.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AEquipmentActor : public AActor
{
	GENERATED_BODY()

	friend class UInventoryManager;

public:

	AEquipmentActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobject)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnBeginUse();
	virtual void OnBeginUse_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnEndUse();
	virtual void OnEndUse_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnEquip();
	virtual void OnEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnUnequip();
	virtual void OnUnequip_Implementation() { K2_DestroyActor(); }

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnSaveData(UPARAM(ref) FJsonObjectWrapper& Json);
	virtual void OnSaveData_Implementation(UPARAM(ref) FJsonObjectWrapper& Json) {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnLoadData(const FJsonObjectWrapper& Json);
	virtual void OnLoadData_Implementation(const FJsonObjectWrapper& Json) {}

	UFUNCTION(BlueprintPure, Category = Equipment)
		bool IsBeingUsed() const { return bUsing; }

protected:

	bool bUsing;

	FJsonObjectWrapper UnequipItem();
	void EquipItem(const FJsonObjectWrapper& InMetadata);
	void SetUseState(const bool bInUse);
};
