// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/MiscActions.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/LatentInfo.h"

void FWTMisc_LevelStreaming::RunAction()
{
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Level : Load)
	{
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorldContext(), Level.Key,
			Level.Value, false, LatentInfo::Make());
	}
	for (const TSoftObjectPtr<UWorld>& Level : Unload)
	{
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorldContext(),
			Level, LatentInfo::Make(), false);
	}
}

void FWTMisc_LevelStreaming::OnBeginPlay()
{
	for (auto It = Load.CreateIterator(); It; ++It)
	{
		if (It.Key().IsNull()) It.RemoveCurrent();
	}
	for (auto It = Unload.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}
