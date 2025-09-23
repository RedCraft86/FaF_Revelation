// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Achievements/AchievementManager.h"

#include "ToroRuntime.h"

void UAchievementManager::Grant(const FGameplayTag Key, const uint8 Value)
{
	if (FAchievementEntry* Entry = Database ? Database->GetEntry(Key) : nullptr)
	{
		// TODO save
	}
}

uint8 UAchievementManager::GetProgress(const FGameplayTag Key)
{
	if (FAchievementEntry* Entry = Database ? Database->GetEntry(Key) : nullptr)
	{
		// TODO save
	}
	return 0;
}

bool UAchievementManager::HasAchievement(const FGameplayTag Key)
{
	if (const FAchievementEntry* Entry = Database ? Database->GetEntry(Key) : nullptr)
	{
		return GetProgress(Key) >= Entry->Requirement;
	}
	return false;
}

void UAchievementManager::BeginPlay()
{
	Super::BeginPlay();
	Database = UToroSettings::Get()->GetDatabase<UAchievementDatabase>();
#if WITH_EDITOR
	if (!Database)
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Achievement Database not found in settings!"))
	}
#endif
}
