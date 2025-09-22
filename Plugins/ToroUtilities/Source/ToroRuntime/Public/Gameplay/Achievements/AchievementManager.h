// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroGameState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "AchievementManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UAchievementManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UAchievementManager() {}
	
	GLOBAL_COMPONENT_GETTER(UAchievementManager, AToroGameState, Achievements)

	// TODO
};
