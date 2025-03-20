// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/ToroNarrative.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"
#include "Narrative/NarrativeWidget.h"

UToroNarrativeComponent* UToroNarrativeComponent::Get(const UObject* WorldContext)
{
	if (const AToroPlayerController* Controller = AToroPlayerController::Get(WorldContext))
	{
		return Controller->GetNarrative();
	}
	return nullptr;
}

void UToroNarrativeComponent::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	PlayerChar->EnterDialogue();
	PlayerChar->AddLockFlag(Tag_PlayerLock_Dialogue.GetTag());
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		CachedInputMode = Controller->GetInputModeData();
		Controller->SetGameInputMode(EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, GetWidget());
	}
}

void UToroNarrativeComponent::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);
	PlayerChar->ClearLockFlag(Tag_PlayerLock_Dialogue.GetTag());
	AToroPlayerController::Get(this)->SetInputModeData(CachedInputMode);
}

UNarrativeWidget* UToroNarrativeComponent::GetWidget()
{
	if (Widget) return Widget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		Widget = Manager->FindWidget<UNarrativeWidget>();
	}
	return Widget;
}

void UToroNarrativeComponent::BeginPlay()
{
	Super::BeginPlay();
}
