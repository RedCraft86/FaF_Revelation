// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Helpers/GameplayTagMacros.h"
#include "InventoryManager.generated.h"

namespace InventoryTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Inventory)
	TORORUNTIME_API inline bool IsValidTag(const FGameplayTag& InTag)
	{
		static FGameplayTag BaseTag = TAG_Inventory.GetTag();
		return InTag.MatchesTag(BaseTag) || InTag == BaseTag;
	}
}

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInventoryManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryManager() {}
};
