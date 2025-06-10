// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PlayerLockFlag.h"

namespace PlayerLockTags
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, MainMenu)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Startup)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
}

bool FPlayerLockFlag::DedupeFlags(TArray<FPlayerLockFlag>& Flags)
{
	if (Flags.Num() < 2)
		return false;
	
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
