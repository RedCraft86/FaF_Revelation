// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/ToroNarrative.h"
#include "Narrative/NarrativeWidget.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerState.h"

UToroNarrative* UToroNarrative::GetNarrative(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UToroNarrative)
}

void UToroNarrative::ToggleQuests() const
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		if (UQuestWidget* WidgetPtr = Manager->FindWidget<UQuestWidget>())
		{
			WidgetPtr->SetHidden(!WidgetPtr->WantsHidden());
		}
	}
}

void UToroNarrative::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	if (UDialogueWidget* Widget = GetDialogueWidget())
	{
		Widget->ActivateWidget();
		Player->AddLockTag(PlayerLockTag::TAG_Dialogue);
		if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
		{
			CachedInput = Controller->GetInputModeData();
			Controller->SetInputModeData({EGameInputMode::GameAndUI, true,
				EMouseLockMode::LockAlways, false, Widget});
		}
	}
}

void UToroNarrative::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);

	if (!bStartingNewDialogue) return;
	if (UDialogueWidget* Widget = GetDialogueWidget())
	{
		Widget->DeactivateWidget();
		Player->ClearLockTag(PlayerLockTag::TAG_Dialogue);
		if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
		{
			Controller->SetInputModeData(CachedInput);
		}
	}
}

UDialogueWidget* UToroNarrative::GetDialogueWidget()
{
	if (DialogueWidget) return DialogueWidget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		DialogueWidget = Manager->FindWidget<UDialogueWidget>();
	}
	return DialogueWidget;
}

void UToroNarrative::BeginPlay()
{
	Super::BeginPlay();
	Player = AToroPlayerCharacter::Get(this);
}
