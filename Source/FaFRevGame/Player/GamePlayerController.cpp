// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayerController.h"
#include "MusicSystem/GameMusicManager.h"

bool AGamePlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
	if (UGameMusicManager* Manager = UGameMusicManager::Get(this))
	{
		bPause ? Manager->AddDipRequest(this) : Manager->RemoveDipRequest(this);
	}
	return Super::SetPause(bPause, CanUnpauseDelegate);
}
