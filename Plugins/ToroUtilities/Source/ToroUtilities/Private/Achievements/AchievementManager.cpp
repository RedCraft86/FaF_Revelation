// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "ToroSettings.h"

void UAchievementManager::Grant(const FGameplayTag InTag, const uint8 Value)
{
	if (!VerifyAchievementTag(InTag) || Value == 0) return;
	if (UToroGlobalSave* Save = GetSaveObject())
	{
		const uint8 Progress = FMath::Min(200, Save->Achievements.FindRef(InTag) + Value);
		Save->Achievements.Add(InTag, Progress);
		Save->SaveObject(nullptr);

		const FAchievementEntry Entry = Database ? Database->Entries.FindRef(InTag) : FAchievementEntry();
		if (Entry.IsValidData() && Progress == Entry.Requirement)
		{
			UGameplayStatics::PlaySound2D(this, Database->Sound.LoadSynchronous());
			if (OnAchievement.IsBound()) OnAchievement.Execute(Entry, Progress);
		}
	}
}

uint8 UAchievementManager::GetProgress(const FGameplayTag InTag)
{
	if (!VerifyAchievementTag(InTag)) return 0;
	if (const UToroGlobalSave* Save = GetSaveObject())
	{
		return Save->Achievements.FindRef(InTag);
	}
	
	return 0;
}

bool UAchievementManager::HasAchievement(const FGameplayTag InTag)
{
	if (!VerifyAchievementTag(InTag)) return false;
	return GetProgress(InTag) > Database->Entries.FindRef(InTag).Requirement;
}

UToroGlobalSave* UAchievementManager::GetSaveObject()
{
	if (SaveObject) return SaveObject;
	SaveObject = ToroSave::GetObject<UToroGlobalSave>(this, Tag_Save_Global);
	return SaveObject;
}

bool UAchievementManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && !UToroSettings::Get()->AchievementDatabase.IsNull();
}

void UAchievementManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Database = UToroSettings::Get()->AchievementDatabase.LoadSynchronous();
}
