// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"

#define UE_DECLARE_GAMEPLAY_TAG_BASE(TagName) UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_##TagName) \
    inline bool Verify##TagName##Tag(const FGameplayTag& InTag) { return InTag.IsValid() && InTag != Tag_##TagName; }
#define UE_DEFINE_GAMEPLAY_TAG_BASE(TagName) UE_DEFINE_GAMEPLAY_TAG(Tag_##TagName, #TagName)
#define GAMEPLAY_TAG_BASE(TagName) FGameplayTag::RequestGameplayTag(#TagName)

#define UE_DECLARE_GAMEPLAY_TAG_CHILD(TagName, BaseTag) UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_##BaseTag##_##TagName)
#define UE_DEFINE_GAMEPLAY_TAG_CHILD(TagName, BaseTag) UE_DEFINE_GAMEPLAY_TAG(Tag_##BaseTag##_##TagName, #BaseTag"."#TagName)
#define GAMEPLAY_TAG_CHILD(TagName, BaseTag) FGameplayTag::RequestGameplayTag(#BaseTag"."#TagName)

UE_DECLARE_GAMEPLAY_TAG_BASE(Weather)

DECLARE_LOG_CATEGORY_EXTERN(LogToroCore, All, All);

class FToroCoreModule final : public IModuleInterface
{
public:
    
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};
