// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InspectionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInspectionManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UInspectionManager();
};
