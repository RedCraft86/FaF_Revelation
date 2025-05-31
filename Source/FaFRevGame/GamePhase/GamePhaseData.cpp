// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhaseData.h"
#include "GamePhaseManager.h"

DEFINE_GAMEPLAY_TAG_BASE(Content)

UGamePhaseGraph::UGamePhaseGraph(): WidgetDelay(0.25f)
{
	CompatibleNode = UGamePhaseNode::StaticClass();
	CompatibleEdge = nullptr;
}

UGamePhaseNode::UGamePhaseNode(): PlayerControl(0) // TODO set default
{
	Name = NSLOCTEXT("Game", "NewSectionName", "New Section");
	Description = NSLOCTEXT("Game", "NewSectionDesc", "A node containing data relevant to a section of the game.");
#if WITH_EDITORONLY_DATA
	ContextMenuName = NSLOCTEXT("Game", "GameSection", "Game Section");
#endif
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
	if (StartSequence.LoadSynchronous()) StartSequence->OnSequenceFinished().Unbind();
	if (FinishSequence.LoadSynchronous()) FinishSequence->OnSequenceFinished().Unbind();
}

void UGamePhaseNode::BindSequenceEvents(UGamePhaseManager* Target) const
{
	if (Target)
	{
		if (StartSequence.LoadSynchronous())
		{
			StartSequence->OnSequenceFinished().BindUObject(Target, &UGamePhaseManager::OnStartSequenceEnd);
		}
		if (FinishSequence.LoadSynchronous())
		{
			FinishSequence->OnSequenceFinished().BindUObject(Target, &UGamePhaseManager::OnFinishSequenceEnd);
		}
	}
}

bool UGamePhaseNode::PlaySequence(const TSoftObjectPtr<AToroSequenceActor>& InSequence)
{
	if (InSequence.LoadSynchronous()) InSequence->Play();
	return InSequence.IsValid();
}

#if WITH_EDITOR
void UGamePhaseNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!MainLevel.IsNull()) Levels.Remove(MainLevel);
	if (Quest.LoadSynchronous() == UQuest::StaticClass()) Quest.Reset();
	for (TPair<TSoftObjectPtr<UInventoryItemData>, uint8>& Item : Items)
	{
		Item.Value = FMath::Max(1, (int32)Item.Value);
	}
}
#endif