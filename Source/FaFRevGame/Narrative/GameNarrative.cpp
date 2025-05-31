// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameNarrative.h"
#include "Player/GamePlayer.h"
#include "Framework/ToroWidgetManager.h"

UGameNarrative* UGameNarrative::Get(const UObject* ContextObject)
{
	AGamePlayer* PlayerChar = AGamePlayer::Get<AGamePlayer>(ContextObject);
	return PlayerChar ? PlayerChar->Narrative : nullptr;
}

void UGameNarrative::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	Player->AddLockFlag(GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue));
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
		Player->ClearLockFlag(GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue));
		if (AToroPlayerController* Controller = AToroPlayerController::Get(this))
		{
			Controller->SetInputModeData(CachedInputMode);
		}
	}
}

UUserWidget* UGameNarrative::GetWidget() const
{
	if (!NarrativeWidget) return nullptr;
	AToroWidgetManager* Manager = AToroWidgetManager::Get(this);
	return Manager ? Manager->FindOrAddWidget(NarrativeWidget) : nullptr;
}

void UGameNarrative::BeginPlay()
{
	Super::BeginPlay();
	Player = AGamePlayer::Get<AGamePlayer>(this);
	GetWidget(); // Just to create the widget
}
