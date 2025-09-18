// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroCheatManager.h"

AToroPlayerController::AToroPlayerController()
{
	PlayerCameraManagerClass = AToroCameraManager::StaticClass();
	CheatClass = UToroCheatManager::StaticClass();
}
