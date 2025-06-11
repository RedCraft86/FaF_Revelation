// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PlayerLockTag.h"

namespace PlayerLockTag
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, MainMenu)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Startup)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, GameOver)
}
