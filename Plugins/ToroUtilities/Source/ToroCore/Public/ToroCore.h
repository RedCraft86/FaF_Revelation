// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"

#define UE_DECLARE_GAMEPLAY_TAG_BASE(TagName) \
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_##TagName) \
    inline bool Verify##TagName##Tag(const FGameplayTag& InTag) { return InTag.IsValid() && InTag != Tag_##TagName; }

#define UE_DEFINE_GAMEPLAY_TAG_BASE(TagName) \
    UE_DEFINE_GAMEPLAY_TAG(Tag_##TagName, #TagName)

UE_DECLARE_GAMEPLAY_TAG_BASE(Weather)
UE_DECLARE_GAMEPLAY_TAG_BASE(Character)

DECLARE_LOG_CATEGORY_EXTERN(LogToroCore, All, All);

class FToroCoreModule final : public IModuleInterface
{
public:
    
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};
