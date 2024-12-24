// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphEd.h"
#include "AssetToolsModule.h"
#include "DataGraphNodeFactory.h"
#include "DataGraphAssetTypeActions.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

DEFINE_LOG_CATEGORY(DataGraphEditor);

#define LOCTEXT_NAMESPACE "DataGraphEditor"

void FDataGraphEdModule::StartupModule()
{
    FDataGraphEdStyle::Init();

    DataGraphNodeFactory = MakeShareable(new FDataGraphNodeFactory());
    if (DataGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::RegisterVisualNodeFactory(DataGraphNodeFactory);
    }

    if (const FAssetToolsModule* AssetToolsModule = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools"))
    {
        DataGraphAction = MakeShareable(new FDataGraphAssetTypeActions());
        AssetToolsModule->Get().RegisterAssetTypeActions(DataGraphAction.ToSharedRef());
    }
}

void FDataGraphEdModule::ShutdownModule()
{
    FDataGraphEdStyle::Shutdown();
    if (const FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools"))
    {
        if (DataGraphAction.IsValid()) AssetToolsModule->Get().UnregisterAssetTypeActions(DataGraphAction.ToSharedRef());
    }

    if (DataGraphNodeFactory.IsValid())
    {
        FEdGraphUtilities::UnregisterVisualNodeFactory(DataGraphNodeFactory);
        DataGraphNodeFactory.Reset();
    }
}

#define RootToContentDir StyleSet->RootToContentDir
TSharedPtr<FSlateStyleSet> FDataGraphEdStyle::StyleSet = nullptr;
void FDataGraphEdStyle::Init()
{
    if (StyleSet.IsValid()) return;
    StyleSet = MakeShareable(new FSlateStyleSet("DataGraphEdStyle"));
    StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources/Icons"));
    
    StyleSet->Set("ClassThumbnail.GenericDataGraph", new IMAGE_BRUSH_SVG(TEXT("DataGraphIcon"), FVector2D(64.0f)));
    StyleSet->Set("DataGraph.AutoArrangeNodes", new IMAGE_BRUSH_SVG(TEXT("DataGraphArrange"), FVector2D(20.0f)));
	
    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FDataGraphEdStyle::Shutdown()
{
    if (StyleSet.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
        ensure(StyleSet.IsUnique());
        StyleSet.Reset();
    }
}
#undef RootToContentDir

IMPLEMENT_MODULE(FDataGraphEdModule, DataGraphEd)

#undef LOCTEXT_NAMESPACE