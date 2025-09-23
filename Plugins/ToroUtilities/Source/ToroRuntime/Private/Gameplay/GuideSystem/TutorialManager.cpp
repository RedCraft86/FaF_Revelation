// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Tutorials/TutorialManager.h"
#include "ToroRuntime.h"

bool UTutorialManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && !UToroSettings::Get()->IsOnMap(Outer, EToroMapType::MainMenu);
}

bool UTutorialManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}
