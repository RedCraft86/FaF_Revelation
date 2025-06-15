// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "InventoryEquipment.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API AInventoryEquipment final : public AActor
{
	GENERATED_BODY()

	friend class UInventoryComponent;

public:

	AInventoryEquipment()
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = false;
		SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
		SetRootComponent(SceneRoot);
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobject)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginUse();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEndUse();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnquip();

	UFUNCTION(BlueprintPure, Category = Equipment)
		bool IsBeingUsed() const { return bUsing; }

private:

	bool bUsing;
	void SetUseState(const bool bInUse)
	{
		if (bUsing != bInUse)
		{
			bUsing = bInUse;
			bUsing ? OnBeginUse() : OnEndUse();
		}
	}
};
