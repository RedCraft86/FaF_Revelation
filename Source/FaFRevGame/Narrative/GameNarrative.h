// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "GameNarrative.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class FAFREVGAME_API UGameNarrativeComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGameNarrativeComponent();

	void ToggleQuests();
};
