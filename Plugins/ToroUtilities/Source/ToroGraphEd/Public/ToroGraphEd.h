// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogDataGraphEditor, All, All);

class FToroGraphEdModule final : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

    TSharedPtr<struct FGraphPanelNodeFactory> DataGraphNodeFactory;
};

class TOROGRAPHED_API FToroGraphEdStyle final
{
public:
    
    static void Init();
    static void Shutdown();
    static const ISlateStyle& Get() { return *StyleSet; }
    static const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }

private:
    
    static TSharedPtr<FSlateStyleSet> StyleSet;
};

namespace ToroGraphColors
{
    static constexpr FLinearColor InvalidBackground(0.4f, 0.0f, 0.0f);
    static constexpr FLinearColor DefaultPin(0.02f, 0.02f, 0.02f);
}