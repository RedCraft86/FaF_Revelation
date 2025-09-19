// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroGameState.h"
#include "Framework/ToroPlayerController.h"
#include "Player/ToroPlayerCharacter.h"
#include "Player/ToroPlayerState.h"
#include "UserInterface/ToroWidgetManager.h"

AToroGameMode::AToroGameMode()
{
	GameStateClass = AToroGameState::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	HUDClass = AToroWidgetManager::StaticClass();
}
