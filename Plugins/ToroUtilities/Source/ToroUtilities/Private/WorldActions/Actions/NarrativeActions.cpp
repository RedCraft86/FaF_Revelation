// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/NarrativeActions.h"
#include "Misc/ToroNarrative.h"

void FWTaskStartDialogue::RunEvent(const UObject* WorldContext)
{
	if (UToroNarrativeComponent* Narrative = UToroNarrativeComponent::Get(WorldContext))
	{
		Narrative->BeginDialogue(DialogueClass);
	}
}

void FWTaskStartQuest::RunEvent(const UObject* WorldContext)
{
	if (UToroNarrativeComponent* Narrative = UToroNarrativeComponent::Get(WorldContext))
	{
		Narrative->BeginQuest(QuestClass);
	}
}

void FWTaskCompleteTask::RunEvent(const UObject* WorldContext)
{
	if (!TaskObject || Argument.IsEmpty() || Quantity == 0) return;
	if (UToroNarrativeComponent* Narrative = UToroNarrativeComponent::Get(WorldContext))
	{
		Narrative->CompleteNarrativeDataTask(TaskObject, Argument, Quantity);
	}
}
