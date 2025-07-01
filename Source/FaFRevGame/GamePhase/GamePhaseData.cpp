// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhaseData.h"
#include "GamePhaseManager.h"
#include "PlayerChar/PlayerTypes.h"

UGamePhaseGraph::UGamePhaseGraph(): WidgetDelay(0.25f)
{
	CompatibleNode = UToroDataNode::StaticClass();
	CompatibleEdge = nullptr;
}

UGamePhaseNode::UGamePhaseNode(): PlayerControl(Player::DefaultControls)
{
	Name = NSLOCTEXT("Game", "NewPhaseName", "New Phase");
	Description = NSLOCTEXT("Game", "NewPhaseDesc", "A node containing data relevant to a Phase of the game.");
#if WITH_EDITORONLY_DATA
	ContextMenuName = NSLOCTEXT("Game", "GamePhase", "Game Phase");
#endif
}

TSet<FGameplayTag> UGamePhaseNode::GetContentTags() const
{
	return TSet(UnlockContent.GetGameplayTagArray());
}

TSet<TSoftObjectPtr<UWorld>> UGamePhaseNode::GetLevels() const
{
	TSet<TSoftObjectPtr<UWorld>> Result;
	Result.Reserve(Levels.Num() + 1);
	if (!MainLevel.IsNull()) Result.Add(MainLevel);
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Pair : Levels)
	{
		if (!Pair.Key.IsNull()) Result.Add(Pair.Key);
	}
	return Result;
}

void UGamePhaseNode::UnbindSequenceEvents() const
{
	if (StartSequence.LoadSynchronous()) StartSequence->OnFinishedEvent.Clear();
	if (FinishSequence.LoadSynchronous()) FinishSequence->OnFinishedEvent.Clear();
}

void UGamePhaseNode::BindSequenceEvents(UGamePhaseManager* Target) const
{
	if (Target)
	{
		if (StartSequence.LoadSynchronous())
		{
			StartSequence->OnFinishedEvent.AddUObject(Target, &UGamePhaseManager::OnStartSequenceEnd);
		}
		if (FinishSequence.LoadSynchronous())
		{
			FinishSequence->OnFinishedEvent.AddUObject(Target, &UGamePhaseManager::OnFinishSequenceEnd);
		}
	}
}

bool UGamePhaseNode::PlaySequence(const TSoftObjectPtr<AToroSequenceActor>& InSequence)
{
	if (InSequence.LoadSynchronous())
	{
		InSequence->bLockPlayer = false;
		InSequence->Play();
	}
	return InSequence.IsValid();
}

#if WITH_EDITOR
void UGamePhaseNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!MainLevel.IsNull()) Levels.Remove(MainLevel);
	if (Quest.LoadSynchronous() == UQuest::StaticClass()) Quest.Reset();
	for (TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : Items)
	{
		if (Item.Value == 0) Item.Value = 1;
	}
}
#endif