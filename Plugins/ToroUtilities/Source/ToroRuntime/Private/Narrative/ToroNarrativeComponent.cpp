// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/ToroNarrativeComponent.h"
#include "Framework/ToroGameMode.h"

UToroNarrativeComponent* UToroNarrativeComponent::Get(const UObject* WorldContext)
{
	if (const AToroGameMode* GameMode = AToroGameMode::Get(WorldContext))
	{
		return GameMode->Narrative;
	}
	
	return nullptr;
}
