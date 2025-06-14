// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UInteractionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();
};
