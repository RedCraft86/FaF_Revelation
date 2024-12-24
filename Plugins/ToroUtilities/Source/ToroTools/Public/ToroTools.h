// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToroToolsModule final : public IModuleInterface
{
public:
    
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

    void RegisterMenus();
    TSharedPtr<FUICommandList> PluginCommands;
};

class TOROTOOLS_API FToroToolsEdStyle final
{
public:
    
    static void Init();
    static void Shutdown();
    static const ISlateStyle& Get() { return *StyleSet; }
    static const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }

private:
    
    static TSharedPtr<FSlateStyleSet> StyleSet;
};