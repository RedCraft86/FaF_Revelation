// Copyright (C) RedCraft86. All Rights Reserved.

#include "Tutorial/TutorialDatabase.h"
#include "ToroSystems.h"

namespace TutorialTags
{
	DEFINE_GAMEPLAY_TAG(Tutorial)
}

bool UTutorialDatabase::IsValidKey(const FGameplayTag& Key)
{
	if (!TutorialTags::IsValidTag(Key)) return false;
	const UTutorialDatabase* Database = UToroSysSettings::Get()->TutorialDB.LoadSynchronous();
	return Database && Database->Entries.Contains(Key) ? Database->Entries.FindRef(Key).IsValidData() : false;
}

FTutorialEntry UTutorialDatabase::Get(const FGameplayTag& Key)
{
	if (!TutorialTags::IsValidTag(Key)) return {};
	const UTutorialDatabase* Database = UToroSysSettings::Get()->TutorialDB.LoadSynchronous();
	return Database ? Database->Entries.FindRef(Key) : FTutorialEntry();
}

#if WITH_EDITOR
int32 UTutorialDatabase::GetValidData() const
{
	int32 Count = 0;
	for (const TPair<FGameplayTag, FTutorialEntry>& Entry : Entries)
	{
		if (TutorialTags::IsValidTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Count++;
		}
	}

	return Count;
}

void UTutorialDatabase::UpdateData()
{
	Super::UpdateData();
	for (TPair<FGameplayTag, FTutorialEntry>& Entry : Entries)
	{
		if (!TutorialTags::IsValidTag(Entry.Key))
		{
			Entry.Value.Label = TEXT("INVALID Tag");
		}
		else if (!Entry.Value.IsValidData())
		{
			Entry.Value.Label = TEXT("INVALID Data");
		}
		else
		{
			Entry.Value.Label = Entry.Value.CustomWidget.IsNull() ?
				Entry.Value.Title.ToString() : Entry.Value.CustomWidget.GetAssetName();
		}
	}
}
#endif
