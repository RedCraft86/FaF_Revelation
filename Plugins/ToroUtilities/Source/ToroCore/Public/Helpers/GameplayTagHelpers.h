// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"

#define GAMEPLAY_TAG_BASE(TagName) FGameplayTag::RequestGameplayTag(#TagName)
#define DEFINE_GAMEPLAY_TAG_BASE(TagName) UE_DEFINE_GAMEPLAY_TAG(Tag_##TagName, #TagName)
#define DECLARE_GAMEPLAY_TAG_BASE(TagName) UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_##TagName) \
	inline bool Verify##TagName##Tag(const FGameplayTag& InTag) { return InTag.IsValid() && InTag != FGameplayTag::RequestGameplayTag(#TagName); }

#define GAMEPLAY_TAG_CHILD(BaseTag, TagName) FGameplayTag::RequestGameplayTag(#BaseTag"."#TagName)
#define DEFINE_GAMEPLAY_TAG_CHILD(BaseTag, TagName) UE_DEFINE_GAMEPLAY_TAG(Tag_##BaseTag##_##TagName, #BaseTag"."#TagName)
#define DECLARE_GAMEPLAY_TAG_CHILD(BaseTag, TagName) UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_##BaseTag##_##TagName)