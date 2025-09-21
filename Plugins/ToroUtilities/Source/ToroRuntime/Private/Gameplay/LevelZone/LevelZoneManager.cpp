// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/LevelZone/LevelZoneManager.h"

bool ULevelZoneManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsOnMap(Outer, EToroMapType::Gameplay);
}

bool ULevelZoneManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}
