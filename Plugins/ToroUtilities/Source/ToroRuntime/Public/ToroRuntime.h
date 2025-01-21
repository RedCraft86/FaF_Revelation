// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroRuntime, All, All);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Zone);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LocalSound);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Character);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Player);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Friendly);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Enemy);

class FToroRuntimeModule final : public IModuleInterface
{
public:
    
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};
