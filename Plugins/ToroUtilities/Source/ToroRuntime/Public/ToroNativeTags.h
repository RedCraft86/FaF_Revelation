// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTags.h"
#include "NativeGameplayTags.h"

#define GAMEPLAY_TAG(Name) FGameplayTag::RequestGameplayTag(#Name)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Saves);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GlobalSave);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GameSave);
inline bool VerifySaveTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Saves;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Characters);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Friendly);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Enemy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Player);
inline bool VerifyCharacterTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Characters;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_PlayerLock);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockMainMenu);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockStartup);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockLoading);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockCinematic);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockDialogue);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockJumpscare);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockInventory);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockHiding);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockDevice);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockGuide);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LockQTE);
inline bool VerifyPlayerLockTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_PlayerLock;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvMeta);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvNameID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvDescID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvMeshID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvKeyID);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvReuseKey);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_InvJsonData);
inline bool VerifyInvMetaTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_InvMeta;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Achievements);
inline bool VerifyAchievementTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Achievements;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GuidePages);
inline bool VerifyGuidePageTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_GuidePages;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LocalSounds);
inline bool VerifyLocalSoundTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_LocalSounds;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Zone);
inline bool VerifyZoneTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Zone;
}

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Content);
inline bool VerifyContentTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Content;
}