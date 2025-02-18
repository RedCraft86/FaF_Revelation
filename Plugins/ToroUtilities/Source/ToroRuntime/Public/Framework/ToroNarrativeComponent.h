// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "ToroNarrativeComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UToroNarrativeComponent : public UNarrativeComponent
{
	GENERATED_BODY()

public:
	
	UToroNarrativeComponent() {}
	static UToroNarrativeComponent* Get(const UObject* WorldContext);
};
