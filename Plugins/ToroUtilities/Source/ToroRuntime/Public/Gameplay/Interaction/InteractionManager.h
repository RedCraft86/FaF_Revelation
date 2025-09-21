// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerCharacter.h"
#include "InteractionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInteractionManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionManager() {}
	
	PLAYER_COMPONENT_GETTER(UInteractionManager, AToroPlayerCharacter, Interaction)
};
