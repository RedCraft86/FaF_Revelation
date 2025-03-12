// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Saves);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GlobalSave);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_GameSave);

inline bool VerifySaveTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Saves;
}