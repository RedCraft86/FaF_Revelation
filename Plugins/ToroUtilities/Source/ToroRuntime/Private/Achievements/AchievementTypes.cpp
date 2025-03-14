// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementTypes.h"
#include "ToroRuntimeSettings.h"

bool UAchievementDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return VerifyAchievementTag(Key) && Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
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
int32 UAchievementDatabase::GetTotalData() const
{
	return Entries.Num();
}

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
#endif