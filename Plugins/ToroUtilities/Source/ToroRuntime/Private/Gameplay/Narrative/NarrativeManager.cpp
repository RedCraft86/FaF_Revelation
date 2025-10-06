// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Narrative/NarrativeManager.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerCharacter.h"

void UNarrativeManager::ToggleQuests()
{
	if (UQuestWidget* Widget = GetQuestWidget())
	{
		Widget->SetHidden(!Widget->WantsHidden());
	}
}

void UNarrativeManager::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	if (UDialogueWidget* Widget = GetDialogueWidget())
	{
		Widget->PushWidget();
	}
	if (AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(this))
	{
		// TODO begin lock on
		Player->AddLockTag(PlayerLockTags::TAG_Dialogue.GetTag());
	}
}

void UNarrativeManager::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);
	if (UDialogueWidget* Widget = GetDialogueWidget())
	{
		Widget->PopWidget();
	}
	if (AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(this))
	{
		// TODO end lock on
		Player->ClearLockTag(PlayerLockTags::TAG_Dialogue.GetTag());
	}
}

UQuestWidget* UNarrativeManager::GetQuestWidget()
{
	if (!QuestWidget)
	{
		QuestWidget = AToroWidgetManager::GetWidget<UQuestWidget>(this);
	}
	return QuestWidget;
}

UDialogueWidget* UNarrativeManager::GetDialogueWidget()
{
	if (!DialogueWidget)
	{
		DialogueWidget = AToroWidgetManager::GetWidget<UDialogueWidget>(this);
	}
	return DialogueWidget;
}
