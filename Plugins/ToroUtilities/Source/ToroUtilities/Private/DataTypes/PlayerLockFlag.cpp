// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PlayerLockFlag.h"

DEFINE_GAMEPLAY_TAG_BASE(PlayerLock)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Inventory)

FPlayerLockFlag::FPlayerLockFlag(): LockTag(Tag_PlayerLock), LockKey(NAME_None) {}
FPlayerLockFlag::FPlayerLockFlag(const FName& InLock): LockTag(Tag_PlayerLock), LockKey(InLock) {}
FPlayerLockFlag::FPlayerLockFlag(const FString& InLock): LockTag(Tag_PlayerLock), LockKey(*InLock) {}
FPlayerLockFlag::FPlayerLockFlag(const FGameplayTag& InLock): LockTag(InLock), LockKey(NAME_None) {}

FName FPlayerLockFlag::Get() const { return VerifyPlayerLockTag(LockTag) ? LockTag.GetTagName() : LockKey; }
bool FPlayerLockFlag::IsValidFlag() const { return !Get().IsNone(); }
FString FPlayerLockFlag::ToString() const { return Get().ToString(); }

#if WITH_EDITOR
void FPlayerLockFlag::ResetTag() { LockTag = Tag_PlayerLock; }
bool FPlayerLockFlag::HasValidTag() const { return VerifyPlayerLockTag(LockTag); }
#endif

bool FPlayerLockFlag::DedupeFlags(TArray<FPlayerLockFlag>& Flags)
{
	bool bChanged = false;
	TSet<FName> Existing;
	for (auto It = Flags.CreateIterator(); It; ++It)
	{
		FPlayerLockFlag Flag = *It;
		if (FName Name = *Flag; Existing.Contains(Name))
		{
			bChanged = true;
			if (Flag.IsValidFlag() && It.GetIndex() == Flags.Num() - 1)
			{
				*It = FPlayerLockFlag();
			}
			else It.RemoveCurrent();
		}
		else Existing.Add(Name);
	}
	return bChanged;
}
