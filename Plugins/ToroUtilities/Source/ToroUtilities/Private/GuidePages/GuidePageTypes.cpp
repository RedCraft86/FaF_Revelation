// Copyright (C) RedCraft86. All Rights Reserved.

#include "GuidePages/GuidePageTypes.h"
#include "ToroSettings.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(GuidePage)

bool UGuidePageDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return VerifyGuidePageTag(Key) && Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
}

const FGuidePageEntry* UGuidePageDatabase::Find(const FGameplayTag& Key) const
{
	return IsKeyValid(Key) ? Entries.Find(Key) : nullptr;
}

bool UGuidePageDatabase::IsValidKey(const FGameplayTag& Key)
{
	const UGuidePageDatabase* Database = UToroSettings::Get()->GuidePageDatabase.LoadSynchronous();
	return Database ? Database->IsKeyValid(Key) : false;
}

FGuidePageEntry UGuidePageDatabase::Get(const FGameplayTag& Key)
{
	const UGuidePageDatabase* Database = UToroSettings::Get()->GuidePageDatabase.LoadSynchronous();
	const FGuidePageEntry* Entry = Database ? Database->Find(Key) : nullptr;
	return Entry ? *Entry : FGuidePageEntry();
}

#if WITH_EDITOR
int32 UGuidePageDatabase::GetValidData() const
{
	int32 Count = 0;
	for (const TPair<FGameplayTag, FGuidePageEntry>& Entry : Entries)
	{
		if (VerifyGuidePageTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Count++;
		}
	}

	return Count;
}

void UGuidePageDatabase::UpdateData()
{
	Super::UpdateData();
	for (TPair<FGameplayTag, FGuidePageEntry>& Entry : Entries)
	{
		if (!VerifyGuidePageTag(Entry.Key))
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
