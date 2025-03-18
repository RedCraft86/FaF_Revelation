// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementTypes.h"
#include "ToroSettings.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(Achievement)

bool UAchievementDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return VerifyAchievementTag(Key) && Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
}

const FAchievementEntry* UAchievementDatabase::Find(const FGameplayTag& Key) const
{
	return IsKeyValid(Key) ? Entries.Find(Key) : nullptr;
}

bool UAchievementDatabase::IsValidKey(const FGameplayTag& Key)
{
	const UAchievementDatabase* Database = UToroSettings::Get()->AchievementDatabase.LoadSynchronous();
	return Database ? Database->IsKeyValid(Key) : false;
}

FAchievementEntry UAchievementDatabase::Get(const FGameplayTag& Key)
{
	const UAchievementDatabase* Database = UToroSettings::Get()->AchievementDatabase.LoadSynchronous();
	const FAchievementEntry* Entry = Database ? Database->Find(Key) : nullptr;
	return Entry ? *Entry : FAchievementEntry();
}

#if WITH_EDITOR
int32 UAchievementDatabase::GetValidData() const
{
	int32 Count = 0;
	for (const TPair<FGameplayTag, FAchievementEntry>& Entry : Entries)
	{
		if (VerifyAchievementTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Count++;
		}
	}

	return Count;
}

void UAchievementDatabase::UpdateData()
{
	Super::UpdateData();
	for (TPair<FGameplayTag, FAchievementEntry>& Entry : Entries)
	{
		if (!VerifyAchievementTag(Entry.Key))
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
#endif