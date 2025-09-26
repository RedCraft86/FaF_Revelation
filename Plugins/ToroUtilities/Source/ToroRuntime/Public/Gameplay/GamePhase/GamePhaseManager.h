// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroGameState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "GamePhaseManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UGamePhaseManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UGamePhaseManager() {}
	
	GLOBAL_COMPONENT_GETTER(UGamePhaseManager, AToroGameState, GamePhase)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UGamePhaseManager* GetPhaseManager(const UObject* ContextObject)
	{
		return Get(ContextObject);
	}

	// TODO
};
