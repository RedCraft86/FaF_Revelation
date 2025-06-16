// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayerController.h"
#include "MusicSystem/GameMusicManager.h"

bool AGamePlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
	if (AGameMusicManager* MusicManager = AGameMusicManager::Get<AGameMusicManager>(this))
	{
		bPause ? MusicManager->AddDipRequest(this) : MusicManager->RemoveDipRequest(this);
	}
	return Super::SetPause(bPause, CanUnpauseDelegate);
}
