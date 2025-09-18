// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroMusicManager.h"
#include "Framework/ToroPlayerController.h"
#include "Player/ToroPlayerCharacter.h"
#include "Player/ToroPlayerState.h"
#include "Player/ToroWidgetManager.h"

AToroGameMode::AToroGameMode()
{
	GameStateClass = AToroMusicManager::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	HUDClass = AToroWidgetManager::StaticClass();
}
