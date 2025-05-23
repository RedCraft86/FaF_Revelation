// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PlayerLockFlag.h"

DEFINE_GAMEPLAY_TAG_BASE(PlayerLock)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Inventory)

void FPlayerLockFlag::DedupeFlags(TArray<FPlayerLockFlag>& Flags)
{
	TSet<FName> Existing;
	for (auto It = Flags.CreateIterator(); It; ++It)
	{
		if (FName Flag = **It; Existing.Contains(Flag))
		{
			if (It.GetIndex() == Flags.Num() - 1)
			{
				*It = FPlayerLockFlag();
			}
			else It.RemoveCurrent();
		}
		else Existing.Add(Flag);
	}
}