// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameHints/HintDatabase.h"
#include "ToroSystems.h"

namespace HintTags
{
	DEFINE_GAMEPLAY_TAG(Hint)
}

bool UHintDatabase::IsValidKey(const FGameplayTag& Key)
{
	if (!HintTags::IsValidTag(Key)) return false;
	const UHintDatabase* Database = UToroSysSettings::Get()->HintDB.LoadSynchronous();
	return Database && Database->Entries.Contains(Key) ? Database->Entries.FindRef(Key).IsValidData() : false;
}

FHintEntry UHintDatabase::Get(const FGameplayTag& Key)
{
	if (!HintTags::IsValidTag(Key)) return {};
	const UHintDatabase* Database = UToroSysSettings::Get()->HintDB.LoadSynchronous();
	return Database ? Database->Entries.FindRef(Key) : FHintEntry();
}

#if WITH_EDITOR
int32 UHintDatabase::GetValidData() const
{
	int32 Count = 0;
	for (const TPair<FGameplayTag, FHintEntry>& Entry : Entries)
	{
		if (TutorialTags::IsValidTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Count++;
		}
	}

	return Count;
}

void UHintDatabase::UpdateData()
{
	for (TPair<FGameplayTag, FHintEntry>& Entry : Entries)
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
			Entry.Value.Label = Entry.Key.ToString();
		}
	}
}
#endif
