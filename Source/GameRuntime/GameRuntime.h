// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FGameRuntimeModule final : public IModuleInterface
{
private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
