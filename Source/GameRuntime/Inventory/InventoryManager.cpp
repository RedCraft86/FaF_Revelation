// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InventoryManager.h"
#include "GameState/SessionState.h"
#include "InvArchiveDatabase.h"
#include "InvItemDatabase.h"

bool IsValidTag(const FGameplayTag& Tag)
{
	// Is not NONE, and a subtype that isn't a subheader
	return Tag.IsValid() 
		&& Tag != TAG_InventoryArchive.GetTag() 
		&& Tag != TAG_InventoryItem.GetTag()
		&& Tag != TAG_InvEquipment.GetTag() 
		&& Tag != TAG_InvMission.GetTag() 
		&& Tag != TAG_InvKey.GetTag();
}

bool IsItemTag(const FGameplayTag& Tag)
{
	return Tag.MatchesTag(TAG_InventoryItem.GetTag());
}

bool IsArchiveTag(const FGameplayTag& Tag)
{
	return Tag.MatchesTag(TAG_InventoryArchive.GetTag());
}

UInventoryManager::UInventoryManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
}

UInventoryManager* UInventoryManager::Get(const UObject* ContextObject)
{
	const ASessionState* State = ASessionState::Get<ASessionState>(ContextObject);
	return IsValid(State) ? State->GetInventoryManager() : nullptr;
}

bool UInventoryManager::AddItem(const FGameplayTag& Item, const bool bSort)
{
	if (IsValidTag(Item) && IsItemTag(Item) && !Items.Contains(Item))
	{
		Items.Add(Item);
		if (bSort)
		{
			SortItems();
		}

		return true;
	}

	return false;
}

bool UInventoryManager::RemoveItem(const FGameplayTag& Item)
{
	if (IsValidTag(Item) && IsItemTag(Item))
	{
		const int32 Idx = Items.Find(Item);
		if (Idx != INDEX_NONE)
		{
			Items.RemoveAt(Idx);
			return true;
		}
	}

	return false;
}

bool UInventoryManager::HasItem(const FGameplayTag& Item) const
{
	return IsValidTag(Item) && IsItemTag(Item) && Items.Contains(Item);
}

bool UInventoryManager::AddArchive(const FGameplayTag& Archive)
{
	if (IsValidTag(Archive) && IsArchiveTag(Archive) && !Archives.Contains(Archive))
	{
		Archives.Add(Archive);
		return true;
	}

	return false;
}

bool UInventoryManager::HasArchive(const FGameplayTag& Archive) const
{
	return IsValidTag(Archive) && IsArchiveTag(Archive) && Archives.Contains(Archive);
}

void UInventoryManager::EnsureEntries(const TArray<FGameplayTag>& Entries)
{
	for (const FGameplayTag& Entry : Entries)
	{
		if (IsValidTag(Entry))
		{
			if (IsItemTag(Entry))
			{
				if (!Items.Contains(Entry))
				{
					Items.Add(Entry);
				}
			}
			else if (IsArchiveTag(Entry))
			{
				if (!Archives.Contains(Entry))
				{
					Archives.Add(Entry);
				}
			}
		}
	}

	SortItems();
}

TArray<FName> UInventoryManager::ExportArchives() const
{
	TArray<FName> NameArchive;
	NameArchive.Reserve(Archives.Num());

	for (const FGameplayTag& Entry : Archives)
	{
		NameArchive.Add(Entry.GetTagName());
	}

	return NameArchive;
}

void UInventoryManager::ImportArchives(const TArray<FName>& Keys)
{
	Archives.Empty(Keys.Num());
	for (const FName& Key : Keys)
	{
		Archives.Add(FGameplayTag::RequestGameplayTag(Key));
	}
}
