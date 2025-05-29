// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscClasses/ToroNarrativeComponent.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroWidgetManager.h"

UToroNarrativeComponent* UToroNarrativeComponent::Get(const UObject* WorldContext)
{
	if (const AToroPlayerController* PC = AToroPlayerController::Get(WorldContext))
	{
		return PC->GetNarrative();
	}
	return nullptr;
}

void UToroNarrativeComponent::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	if (Controller)
	{
		Player->AddLockFlag(Tag_PlayerLock_Dialogue.GetTag());
		CachedInputMode = Controller->GetInputModeData();
		Controller->SetInputModeData({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, GetWidget()});
	}
}

void UToroNarrativeComponent::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);
	if (!bStartingNewDialogue && Controller)
	{
		Player->ClearLockFlag(Tag_PlayerLock_Dialogue.GetTag());
		Controller->SetInputModeData(CachedInputMode);
	}
}

UUserWidget* UToroNarrativeComponent::GetWidget() const
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(this);
	return Manager ? Manager->FindWidget(WidgetClass) : nullptr;
}

void UToroNarrativeComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = AToroPlayerCharacter::Get(this);
	Controller = GetOwner<AToroPlayerController>();
}
