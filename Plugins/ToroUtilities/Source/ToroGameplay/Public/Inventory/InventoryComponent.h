// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup=(Game), meta=(BlueprintSpawnableComponent))
class TOROGAMEPLAY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInventoryComponent();
};
