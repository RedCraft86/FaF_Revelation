// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroNarrative.h"
#include "Framework/ToroPlayerController.h"

UToroNarrativeComponent* UToroNarrativeComponent::Get(const UObject* WorldContext)
{
	if (const AToroPlayerController* Controller = AToroPlayerController::Get(WorldContext))
	{
		return Controller->GetNarrative();
	}
	
	return nullptr;
}
