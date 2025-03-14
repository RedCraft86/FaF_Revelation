// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "Modules/ModuleManager.h"

class FFaF_RevelationEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
