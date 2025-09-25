// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Narrative/NarrativeManager.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"

void UNarrativeManager::ToggleQuests() const
{
	// TODO ui
}

void UNarrativeManager::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		CachedInputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, nullptr});
		if (AToroPlayerCharacter* Player = PC->GetPawn<AToroPlayerCharacter>())
		{
			Player->AddLockTag(PlayerLockTags::TAG_Dialogue.GetTag());
		}
	}
		// TODO ui
}

void UNarrativeManager::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig(CachedInputConfig);
		CachedInputConfig.ClearAndReset();
		if (AToroPlayerCharacter* Player = PC->GetPawn<AToroPlayerCharacter>())
		{
			Player->ClearLockTag(PlayerLockTags::TAG_Dialogue.GetTag());
		}
	}
		// TODO ui
}
