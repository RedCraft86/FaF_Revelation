// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameRuntime, All, All);

class FGameRuntimeModule final : public IModuleInterface
{
private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
