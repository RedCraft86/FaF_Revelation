// Copyright (C) RedCraft86. All Rights Reserved.

#include "NarrativeActions.h"
#include "GameNarrative.h"

void FWANarrativeDialogue::RunAction()
{
	if (UGameNarrative* Narrative = UGameNarrative::Get(GetWorldContext()))
	{
		Narrative->BeginDialogue(DialogueClass);
	}
}

void FWANarrativeQuest::RunAction()
{
	if (UGameNarrative* Narrative = UGameNarrative::Get(GetWorldContext()))
	{
		Narrative->BeginQuest(QuestClass);
	}
}

void FWANarrativeTask::RunAction()
{
	if (!TaskObject || Argument.IsEmpty() || Quantity == 0) return;
	if (UGameNarrative* Narrative = UGameNarrative::Get(GetWorldContext()))
	{
		Narrative->CompleteNarrativeDataTask(TaskObject, Argument, Quantity);
	}
}
