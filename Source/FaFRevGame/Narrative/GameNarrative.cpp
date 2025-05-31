// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameNarrative.h"
#include "Player/GamePlayer.h"

UGameNarrative* UGameNarrative::Get(const UObject* ContextObject)
{
	AGamePlayer* PlayerChar = AGamePlayer::Get<AGamePlayer>(ContextObject);
	return PlayerChar ? PlayerChar->Narrative : nullptr;
}

void UGameNarrative::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	Player->AddLockFlag(Tag_PlayerLock_Dialogue.GetTag());
	if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
	{
		CachedInputMode = Controller->GetInputModeData();
		Controller->SetInputModeData({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, GetWidget()});
	}
}

void UGameNarrative::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);
	if (!bStartingNewDialogue)
	{
		Player->ClearLockFlag(Tag_PlayerLock_Dialogue.GetTag());
		if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
		{
			Controller->SetInputModeData(CachedInputMode);
		}
	}
}

UUserWidget* UGameNarrative::GetWidget() const
{
	// TODO
	return nullptr;
}

void UGameNarrative::BeginPlay()
{
	Super::BeginPlay();
	Player = AGamePlayer::Get<AGamePlayer>(this);
}
