// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "EquipmentActor.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class AEquipmentActor final : public AActor
{
	GENERATED_BODY()

	friend class UInventoryManager;

public:

	AEquipmentActor();

private:

	UFUNCTION(BlueprintNativeEvent)
		void OnUse();
	void OnUse_Implementation() {}

	virtual void BeginPlay() override;
};
