// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

	void OpenInventory();
	void CloseInventory();
	void SetEquipmentUse(const bool bUsing);
};
