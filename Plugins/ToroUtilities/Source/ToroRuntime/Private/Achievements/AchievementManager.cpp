// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementManager.h"
#include "SaveSystem/ToroSaveManager.h"
#include "ToroRuntimeSettings.h"
#include "Framework/ToroWidgetManager.h"

void UAchievementManager::Grant(const FGameplayTag InTag, const uint8 Value)
{
	if (!InTag.IsValid() || InTag == Tag_Achievements) return;
	if (UGlobalSaveObjectBase* Save = GetSaveObject())
	{
		const uint8 Progress = FMath::Min(200, Save->Achievements.FindRef(InTag) + Value);
		Save->Achievements.Add(InTag, Progress);
		Save->SaveObject(nullptr);

		const FAchievementEntry Entry = Database ? Database->Entries.FindRef(InTag) : FAchievementEntry();
		if (Entry.IsValidData() && Progress == Entry.Requirement)
		{
			if (AToroWidgetManager* WidgetManager = AToroWidgetManager::Get(this))
			{
				WidgetManager->QueueLargeNotice({FText::Format(Database->NotifyFormat, Entry.Name), 0.0f});
				UGameplayStatics::PlaySound2D(this, Database->Sound.LoadSynchronous());
			}
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
	if (UToroSaveManager* SaveSystem = UToroSaveManager::Get(this))
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
