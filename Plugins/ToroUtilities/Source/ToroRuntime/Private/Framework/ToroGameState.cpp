// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameState.h"
#include "MusicSystem/WorldMusicManager.h"
#include "Achievements/AchievementManager.h"

AToroGameState::AToroGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MusicManager = CreateDefaultSubobject<UWorldMusicManager>("MusicManager");
	MusicManager->SetupAttachment(SceneRoot);

	Achievements = CreateDefaultSubobject<UAchievementManager>("Achievements");
}
