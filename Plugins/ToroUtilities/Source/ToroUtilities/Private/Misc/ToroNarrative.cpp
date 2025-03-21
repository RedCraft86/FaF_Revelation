// Copyright (C) RedCraft86. All Rights Reserved.

#include "Misc/ToroNarrative.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroWidgetManager.h"

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
	if (!bStartingNewDialogue)
	{
		PlayerChar->ExitDialogue();
		PlayerChar->ClearLockFlag(Tag_PlayerLock_Dialogue.GetTag());
		AToroPlayerController::Get(this)->SetInputModeData(CachedInputMode);
	}
}

UUserWidget* UToroNarrativeComponent::GetWidget() const
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(this);
	return Manager ? Manager->FindWidget(WidgetClass) : nullptr;
}
