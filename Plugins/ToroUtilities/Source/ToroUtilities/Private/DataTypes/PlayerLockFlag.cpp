﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PlayerLockFlag.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(PlayerLock)
UE_DEFINE_GAMEPLAY_TAG_CHILD(Cinematic, PlayerLock)
UE_DEFINE_GAMEPLAY_TAG_CHILD(Dialogue, PlayerLock)
UE_DEFINE_GAMEPLAY_TAG_CHILD(Inventory, PlayerLock)

FPlayerLockFlag::FPlayerLockFlag() : LockTag(Tag_PlayerLock), LockKey(NAME_None) {}
FPlayerLockFlag::FPlayerLockFlag(const FName& InLock) : LockTag(Tag_PlayerLock), LockKey(InLock) {}
FPlayerLockFlag::FPlayerLockFlag(const FGameplayTag& InLock) : LockTag(InLock), LockKey(NAME_None) {}

FName FPlayerLockFlag::Get() const { return VerifyPlayerLockTag(LockTag) ? LockTag.GetTagName() : LockKey; }
#if WITH_EDITOR
void FPlayerLockFlag::ResetTag() { LockTag = Tag_PlayerLock; }
bool FPlayerLockFlag::HasValidTag() const { return VerifyPlayerLockTag(LockTag); }
#endif
