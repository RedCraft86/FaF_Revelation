// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerState.h"
#include "NarrativeManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UNarrativeManager : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UNarrativeManager() {}
	
	PLAYER_COMPONENT_GETTER(UNarrativeManager, AToroPlayerState, Narrative)
};
