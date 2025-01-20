// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroRuntime, All, All);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Character);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Player);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_OneShot);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Zone);

class FToroRuntimeModule final : public IModuleInterface
{
public:
    
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};
