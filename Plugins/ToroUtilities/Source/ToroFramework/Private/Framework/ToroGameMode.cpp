// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroGameState.h"
#include "Framework/ToroPlayerState.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "UserInterface/ToroWidgetManager.h"

AToroGameMode::AToroGameMode()
{
	GameStateClass = AToroGameState::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	HUDClass = AToroWidgetManager::StaticClass();
}
