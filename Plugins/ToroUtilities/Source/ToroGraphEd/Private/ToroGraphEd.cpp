// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGraphEd.h"
#include "EdGraphUtilities.h"
#include "Interfaces/IPluginManager.h"
#include "Nodes/DataGraphNodeFactory.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"

DEFINE_LOG_CATEGORY(LogDataGraphEditor);

#define LOCTEXT_NAMESPACE "FToroGraphEdModule"

void FToroGraphEdModule::StartupModule()
{
    FToroGraphEdStyle::Init();
    DataGraphNodeFactory = MakeShareable(new FDataGraphNodeFactory());
    if (DataGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::RegisterVisualNodeFactory(DataGraphNodeFactory);
    }
}

void FToroGraphEdModule::ShutdownModule()
{
    FToroGraphEdStyle::Shutdown();
    if (DataGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(DataGraphNodeFactory);
        DataGraphNodeFactory.Reset();
    }
}

#define RootToContentDir StyleSet->RootToContentDir
#define AddSVG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH_SVG(TEXT(#ImageFile), FVector2D(Size)));
TSharedPtr<FSlateStyleSet> FToroGraphEdStyle::StyleSet = nullptr;
void FToroGraphEdStyle::Init()
{
    if (StyleSet.IsValid()) return;
    StyleSet = MakeShareable(new FSlateStyleSet("ToroGraphEdStyle"));
    StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources"));

    AddSVG(ClassThumbnail.DataGraphBase, DataGraphIcon, 64.0f)
    AddSVG(ToroGraph.AutoArrangeNodes, DataGraphArrange, 20.0f)
	
    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}
#undef AddSVG
#undef RootToContentDir

void FToroGraphEdStyle::Shutdown()
{
    if (StyleSet.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
        ensure(StyleSet.IsUnique());
        StyleSet.Reset();
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroGraphEdModule, ToroGraphEd)