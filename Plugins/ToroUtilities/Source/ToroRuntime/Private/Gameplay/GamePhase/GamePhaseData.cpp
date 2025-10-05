// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhase/GamePhaseData.h"
#include "Framework/ToroPlayerCharacter.h"
#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

namespace ContentTags
{
	DEFINE_GAMEPLAY_TAG(Content)
}

UGamePhaseGraph::UGamePhaseGraph()
{
	CompatibleNode = UGamePhaseNode::StaticClass();
	CompatibleEdge = nullptr;
}

UGamePhaseNode::UGamePhaseNode()
	: bSimpleLoading(false)
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

TSoftObjectPtr<UTexture2D> UGamePhaseNode::GetBackground() const
{
	if (bSimpleLoading || Backgrounds.IsEmpty())
	{
		return nullptr;
	}
	if (Backgrounds.Num() == 1)
	{
		return Backgrounds[0];
	}
	return Backgrounds[FMath::RandRange(0, Backgrounds.Num() - 1)];
}

void UGamePhaseNode::TeleportPlayer() const
{
	if (TeleportPoint.LoadSynchronous())
	{
		TeleportPoint->TeleportPlayer();
	}
}

void UGamePhaseNode::ApplyPlayerSettings(AToroPlayerCharacter* PlayerChar) const
{
	PlayerChar->SetLightSettings(PlayerLight);
}

#if WITH_EDITOR
void UGamePhaseNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!MainLevel.IsNull()) Levels.Remove(MainLevel);
	if (Quest.LoadSynchronous() == UQuest::StaticClass()) Quest.Reset();
}
#endif