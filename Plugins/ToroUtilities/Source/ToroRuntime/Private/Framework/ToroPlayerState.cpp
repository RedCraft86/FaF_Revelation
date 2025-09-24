// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerState.h"
#include "Inventory/InventoryManager.h"
#include "Narrative/NarrativeManager.h"
#include "Achievements/AchievementManager.h"

AToroPlayerState::AToroPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	Inventory = CreateDefaultSubobject<UInventoryManager>("Inventory");
	Narrative = CreateDefaultSubobject<UNarrativeManager>("Narrative");
	Achievements = CreateDefaultSubobject<UAchievementManager>("Achievements");
}
