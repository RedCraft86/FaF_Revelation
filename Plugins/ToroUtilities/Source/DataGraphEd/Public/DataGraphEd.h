// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(DataGraphEditor, All, All);

class FDataGraphEdModule final : public IModuleInterface
{
public:
    
    void StartupModule() override;
    void ShutdownModule() override;

private:

    TSharedPtr<class IAssetTypeActions> DataGraphAction;
    TSharedPtr<struct FGraphPanelNodeFactory> DataGraphNodeFactory;
};

class DATAGRAPHED_API FDataGraphEdStyle final
{
public:
    
    static void Init();
    static void Shutdown();
    static const ISlateStyle& Get() { return *StyleSet; }
    static const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }

private:
    
    static TSharedPtr<FSlateStyleSet> StyleSet;
};

namespace DataGraphColors
{
    static constexpr FLinearColor InvalidBackground(0.4f, 0.0f, 0.0f);
    static constexpr FLinearColor DefaultPin(0.02f, 0.02f, 0.02f);
}
