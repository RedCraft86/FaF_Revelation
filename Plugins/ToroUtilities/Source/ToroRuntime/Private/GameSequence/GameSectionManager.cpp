﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/GameSectionManager.h"
#include "ToroRuntimeSettings.h"

bool UGameSectionManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroRuntimeSettings::Get()->IsOnGameplayMap(Outer);
}

bool UGameSectionManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}
