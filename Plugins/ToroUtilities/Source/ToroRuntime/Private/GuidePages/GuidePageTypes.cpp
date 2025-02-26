// Copyright (C) RedCraft86. All Rights Reserved.

#include "GuidePages/GuidePageTypes.h"
#include "ToroRuntimeSettings.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_GuidePages, "GuidePages");

bool UGuidePageDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return Key.IsValid() && Key != Tag_GuidePages
		&& Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
}

bool UGuidePageDatabase::IsValidKey(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return false;
	if (const UGuidePageDatabase* Database = UToroRuntimeSettings::Get()->GuidePageDatabase.LoadSynchronous())
	{
		return Database->IsKeyValid(Key);
	}
	
	return false;
}

FGuidePageEntry UGuidePageDatabase::Get(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return {};
	if (const UGuidePageDatabase* Database = UToroRuntimeSettings::Get()->GuidePageDatabase.LoadSynchronous())
	{
		return Database->Entries.FindRef(Key);
	}
	
	return {};
}

#if WITH_EDITOR
int32 UGuidePageDatabase::GetTotalData() const
{
	return Entries.Num();
}

int32 UGuidePageDatabase::GetValidData() const
{
	int32 Count = 0;
	for (const TPair<FGameplayTag, FGuidePageEntry>& Entry : Entries)
	{
		if (Entry.Key.IsValid() && Entry.Key != Tag_GuidePages && Entry.Value.IsValidData())
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
			Entry.Value.Label = Entry.Value.CustomWidget.IsNull() ?
				Entry.Value.Title.ToString() : Entry.Value.CustomWidget.GetAssetName();
		}
	}
}
#endif
