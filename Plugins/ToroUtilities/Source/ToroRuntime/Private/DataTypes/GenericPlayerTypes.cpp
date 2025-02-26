// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/GenericPlayerTypes.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_PlayerLock, "PlayerLock");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockStartup, "PlayerLock.Startup");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockLoading, "PlayerLock.Loading");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockCutscene, "PlayerLock.Cutscene");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockDialogue, "PlayerLock.Dialogue");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockJumpscare, "PlayerLock.Jumpscare");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockInventory, "PlayerLock.Inventory");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockHiding, "PlayerLock.Hiding");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockDevice, "PlayerLock.Device");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockGuide, "PlayerLock.Guide");
UE_DEFINE_GAMEPLAY_TAG(Tag_LockQTE, "PlayerLock.QTE");

FPlayerLockFlag::FPlayerLockFlag() : LockTag(Tag_PlayerLock), LockKey(NAME_None) {}
FPlayerLockFlag::FPlayerLockFlag(const FName& InLock) : LockTag(Tag_PlayerLock), LockKey(InLock) {}

bool FPlayerLockFlag::HasValidTag() const
{
	return LockTag.IsValid() && LockTag != Tag_PlayerLock;
}

#if WITH_EDITOR
void FPlayerLockFlag::ResetTag()
{
	LockTag = Tag_PlayerLock;
}
#endif
