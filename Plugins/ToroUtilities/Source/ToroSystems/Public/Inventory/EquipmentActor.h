// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "JsonObjectWrapper.h"
#include "GameFramework/Actor.h"
#include "EquipmentActor.generated.h"

UCLASS(Abstract)
class TOROSYSTEMS_API AEquipmentActor : public AActor
{
	GENERATED_BODY()

	friend class UInventoryComponent;

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
	virtual void OnUnequip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnDataSave(UPARAM(ref) FJsonObjectWrapper& JsonData);
	virtual void OnDataSave_Implementation(UPARAM(ref) FJsonObjectWrapper& JsonData) {}

	UFUNCTION(BlueprintNativeEvent, Category = Equipment)
		void OnDataLoad(const FJsonObjectWrapper& JsonData);
	virtual void OnDataLoad_Implementation(const FJsonObjectWrapper& JsonData) {}

	UFUNCTION(BlueprintPure, Category = Equipment)
		bool IsBeingUsed() const { return bUsing; }

protected:

	UPROPERTY() bool bUsing;
	UPROPERTY() FJsonObjectWrapper JsonData;

	void SaveData(FString& InJson);
	void LoadData(const FString& InJson);
	void SetUseState(const bool bInUse);
};
