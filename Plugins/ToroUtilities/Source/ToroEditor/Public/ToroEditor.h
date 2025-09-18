// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define REGISTER_VISUALIZER(Component, Visualizer) \
    TSharedPtr<FComponentVisualizer> VizInst_##Component = Visualizer::MakeInstance(); \
    GUnrealEd->RegisterComponentVisualizer(Component::StaticClass()->GetFName(), VizInst_##Component); \
    VizInst_##Component->OnRegister(); \

#define UNREGISTER_VISUALIZER(Component) \
    GUnrealEd->UnregisterComponentVisualizer(Component::StaticClass()->GetFName());

DECLARE_LOG_CATEGORY_EXTERN(LogToroEditor, All, All);

class FToroEditorModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
