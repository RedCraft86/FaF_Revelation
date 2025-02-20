// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementManager.h"
#include "SaveSystem/ToroSaveSystem.h"
#include "ToroRuntimeSettings.h"

void UAchievementManager::Grant(const FGameplayTag InTag, const uint8 Value)
{
	if (!InTag.IsValid() || InTag == Tag_Achievements) return;
	if (UGlobalSaveObjectBase* Save = GetSaveObject())
	{
		const uint8 Progress = FMath::Min(255, Save->Achievements.FindRef(InTag) + Value);
		Save->Achievements.Add(InTag, Progress);
		Save->SaveObject(nullptr);
		
		if (Database && Progress == Database->Entries.FindRef(InTag).Requirement)
		{
			// TODO: Widget stuff
		}
	}
}

uint8 UAchievementManager::GetProgress(const FGameplayTag InTag)
{
	if (!InTag.IsValid() || InTag == Tag_Achievements) return 0;
	if (const UGlobalSaveObjectBase* Save = GetSaveObject())
	{
		return Save->Achievements.FindRef(InTag);
	}
	
	return 0;
}

bool UAchievementManager::HasAchievement(const FGameplayTag InTag)
{
	if (!InTag.IsValid() || InTag == Tag_Achievements) return false;
	return GetProgress(InTag) > Database->Entries.FindRef(InTag).Requirement;
}

UGlobalSaveObjectBase* UAchievementManager::GetSaveObject()
{
	if (SaveObject) return SaveObject;
	if (UToroSaveSystem* SaveSystem = UToroSaveSystem::Get(this))
	{
		SaveObject = SaveSystem->GetSaveObject<UGlobalSaveObjectBase>(Tag_GlobalSave);
	}

	return SaveObject;
}

bool UAchievementManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && !UToroRuntimeSettings::Get()->AchievementDatabase.IsNull();
}

void UAchievementManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Database = UToroRuntimeSettings::Get()->AchievementDatabase.LoadSynchronous();
}
