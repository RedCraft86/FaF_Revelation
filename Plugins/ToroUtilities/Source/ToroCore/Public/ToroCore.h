// Copyright (C) RedCraft86, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToroCoreModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
