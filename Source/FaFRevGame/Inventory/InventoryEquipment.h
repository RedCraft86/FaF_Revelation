// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "InventoryEquipment.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API AInventoryEquipment final : public AActor
{
	GENERATED_BODY()

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
		void UseItem();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnUnquip();
};
