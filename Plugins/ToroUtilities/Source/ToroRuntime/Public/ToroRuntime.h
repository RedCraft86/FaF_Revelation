// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroRuntime, All, All);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Zone);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Background);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_LocalSound);

class FToroRuntimeModule final : public IModuleInterface
{
public:
    
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};
