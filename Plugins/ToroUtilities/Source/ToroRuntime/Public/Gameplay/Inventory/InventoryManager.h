// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerState.h"
#include "InventoryManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryManager() {}
	
	PLAYER_COMPONENT_GETTER(UInventoryManager, AToroPlayerState, Inventory)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UInventoryManager* GetInventoryManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	// TODO
};
