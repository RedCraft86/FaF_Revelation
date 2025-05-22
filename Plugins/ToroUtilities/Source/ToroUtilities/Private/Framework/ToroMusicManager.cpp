// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroMusicManager.h"

AToroMusicManager::AToroMusicManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}
