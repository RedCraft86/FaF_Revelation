// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePhase/GamePhaseData.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Inventory/InventoryManager.h"

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
	: bSimpleLoading(false), InventoryProfile(InventoryTags::TAG_Inventory.GetTag())
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

void UGamePhaseNode::EnsureInventoryItems(UInventoryManager* Inventory) const
{
	for (const TPair<TSoftObjectPtr<UInventoryAsset>, bool>& Archive : Archives)
	{
		if (UInventoryAsset* Asset = Archive.Key.LoadSynchronous())
		{
			Inventory->AddArchive(Asset, Archive.Value);
		}
	}

	FInventoryItems& ItemsRef = Inventory->GetItems();
	for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : Items)
	{
		if (UInventoryAsset* Asset = Item.Value > 0 ? Item.Key.LoadSynchronous() : nullptr)
		{
			if (FInventoryItemSlot* Slot = ItemsRef.FindItem(Asset))
			{
				Slot->Amount = FMath::Max(Slot->Amount, Item.Value);
			}
			else
			{
				ItemsRef.AddItem(Asset, Item.Value);
			}
		}
	}
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