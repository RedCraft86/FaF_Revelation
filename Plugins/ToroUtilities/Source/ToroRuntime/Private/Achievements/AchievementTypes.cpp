// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementTypes.h"
#include "ToroRuntimeSettings.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Achievements, "Achievements");

bool UAchievementDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return Key.IsValid() && Key != Tag_Achievements
		&& Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
}

uint8 UAchievementDatabase::GetValidCount() const
{
	uint8 Count = 0;
	for (const TPair<FGameplayTag, FAchievementEntry>& Entry : Entries)
	{
		if (Entry.Key.IsValid() && Entry.Key != Tag_Achievements && Entry.Value.IsValidData())
		{
			Count++;
		}
	}

	return Count;
}

bool UAchievementDatabase::IsValidKey(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return false;
	if (const UAchievementDatabase* Database = UToroRuntimeSettings::Get()->AchievementDatabase.LoadSynchronous())
	{
		return Database->IsKeyValid(Key);
	}
	
	return false;
}

FAchievementEntry UAchievementDatabase::Get(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return {};
	if (const UAchievementDatabase* Database = UToroRuntimeSettings::Get()->AchievementDatabase.LoadSynchronous())
	{
		return Database->Entries.FindRef(Key);
	}
	
	return {};
}

#if WITH_EDITOR
void UAchievementDatabase::UpdateAchievements()
{
	for (TPair<FGameplayTag, FAchievementEntry>& Entry : Entries)
	{
		if (!Entry.Key.IsValid() || Entry.Key == Tag_Achievements)
		{
			Entry.Value.Label = TEXT("INVALID Tag");
		}
		else if (!Entry.Value.IsValidData())
		{
			Entry.Value.Label = TEXT("INVALID Data");
		}
		else
		{
			Entry.Value.Label = Entry.Value.Name.ToString();
		}
	}
}

void UAchievementDatabase::PostLoad()
{
	Super::PostLoad();
	UpdateAchievements();
}

void UAchievementDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateAchievements();
}
#endif
