// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "TutorialDatabase.h"
#include "Components/ActorComponent.h"
#include "Framework/ToroPlayerState.h"
#include "Helpers/ClassGetterMacros.h"
#include "TutorialManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UTutorialManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UTutorialManager();

	 // TODO
	
	PLAYER_COMPONENT_GETTER(UTutorialManager, AToroPlayerState, Tutorials)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UTutorialManager* GetTutorialManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}
};
