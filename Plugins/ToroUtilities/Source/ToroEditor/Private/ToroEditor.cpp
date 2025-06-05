// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define REGISTER_TOOL(Tool) Tool::Register(PluginCommands);
#define REGISTER_TOOL_MENUS(Tool) Tool::RegisterMenus(PluginCommands);

#define LOCTEXT_NAMESPACE "FToroEditorModule"

void FToroEditorModule::StartupModule()
{
    
}

void FToroEditorModule::ShutdownModule()
{
    
}

void FToroEditorModule::RegisterMenus()
{
}

#define RootToContentDir StyleSet->RootToContentDir
void FToroEditorStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroEditorStyle"));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources"));

	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);

	StyleSet->Set("ToroEditor.LinkAssetLibrary", new IMAGE_BRUSH_SVG(TEXT("LinkFolder"), Icon20x20));
	StyleSet->Set("ToroEditor.RestartEditor", new IMAGE_BRUSH_SVG(TEXT("RestartEditor"), Icon20x20));
	StyleSet->Set("ToroEditor.ActorLayout", new IMAGE_BRUSH_SVG(TEXT("ActorLayout"), Icon20x20));
	StyleSet->Set("ToroEditor.StaticMeshBaker", new IMAGE_BRUSH_SVG(TEXT("StaticMeshBaker"), Icon20x20));
	StyleSet->Set("ToroEditor.StaticMeshMerger", new IMAGE_BRUSH_SVG(TEXT("StaticMeshMerger"), Icon20x20));
	StyleSet->Set("ToroEditor.StaticMeshInstancer", new IMAGE_BRUSH_SVG(TEXT("StaticMeshInstancer"), Icon20x20));

	StyleSet->Set("ClassThumbnail.MasterPostProcess", new IMAGE_BRUSH(TEXT("PostProcess"), Icon64x64));
	StyleSet->Set("ClassThumbnail.LightProbeActor", new IMAGE_BRUSH(TEXT("LightProbe"), Icon64x64));

	if (const TSharedPtr<IPlugin> ExpressiveText = IPluginManager::Get().FindPlugin(TEXT("ExpressiveText")))
	{
		StyleSet->Set("ClassIcon.ExprTextBlock", new FSlateImageBrush(
			ExpressiveText->GetContentDir() / TEXT("Core/Editor/Resources/ExTextLogo.png"),
			Icon16x16
		));
	}
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FToroEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)