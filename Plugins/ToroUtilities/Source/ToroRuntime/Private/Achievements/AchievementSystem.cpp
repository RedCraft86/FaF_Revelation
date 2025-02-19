// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementSystem.h"
#include "ToroRuntimeSettings.h"

bool UAchievementSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && !UToroRuntimeSettings::Get()->AchievementDatabase.IsNull();
}

void UAchievementSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Database = UToroRuntimeSettings::Get()->AchievementDatabase.LoadSynchronous();
}
