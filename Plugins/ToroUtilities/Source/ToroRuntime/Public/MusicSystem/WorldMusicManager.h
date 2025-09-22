// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroGameState.h"
#include "Components/AudioComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "WorldMusicManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (World), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UWorldMusicManager final : public UAudioComponent
{
	GENERATED_BODY()

public:

	UWorldMusicManager() {}
	
	GLOBAL_COMPONENT_GETTER(UWorldMusicManager, AToroGameState, MusicManager)

	// TODO
};
