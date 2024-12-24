// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroTools.h"
#include "ToroToolCommands.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"

#include "Tools/RestartEditorTool.h"
#include "Tools/StaticMeshBakerTool.h"

#define REGISTER_TOOL(Tool) Tool::Register(PluginCommands);
#define REGISTER_TOOL_MENUS(Tool) Tool::RegisterMenus(PluginCommands);
#define LOCTEXT_NAMESPACE "FToroToolsModule"

void FToroToolsModule::StartupModule()
{
	FToroToolsEdStyle::Init();
	FToroToolCommands::Register();
    
	PluginCommands = MakeShareable(new FUICommandList);
	REGISTER_TOOL(FRestartEditorTool);
	REGISTER_TOOL(FStaticMeshBakerTool);

	IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
	MainFrame.GetMainFrameCommandBindings()->Append(PluginCommands.ToSharedRef());
	
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
		this, &FToroToolsModule::RegisterMenus));
}

void FToroToolsModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FToroToolCommands::Unregister();
	FToroToolsEdStyle::Shutdown();
}

void FToroToolsModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		REGISTER_TOOL_MENUS(FRestartEditorTool);
		REGISTER_TOOL_MENUS(FStaticMeshBakerTool);
	}
}

#define RootToContentDir StyleSet->RootToContentDir
TSharedPtr<FSlateStyleSet> FToroToolsEdStyle::StyleSet = nullptr;
void FToroToolsEdStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroToolsEdStyle"));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources/Icons"));

	const FVector2D Icon20x20(20.0f, 20.0f);
	StyleSet->Set("ToroTools.RestartEditor", new IMAGE_BRUSH_SVG(TEXT("RestartEditor"), Icon20x20));
	StyleSet->Set("ToroTools.ChannelPacker", new IMAGE_BRUSH_SVG(TEXT("ChannelPacker"), Icon20x20));
	StyleSet->Set("ToroTools.StaticMeshBaker", new IMAGE_BRUSH_SVG(TEXT("StaticMeshBaker"), Icon20x20));
	StyleSet->Set("ToroTools.StaticMeshMerger", new IMAGE_BRUSH_SVG(TEXT("StaticMeshMerger"), Icon20x20));
	StyleSet->Set("ToroTools.StaticMeshInstancer", new IMAGE_BRUSH_SVG(TEXT("StaticMeshInstancer"), Icon20x20));
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FToroToolsEdStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}
#undef RootToContentDir

IMPLEMENT_MODULE(FToroToolsModule, ToroTools)

#undef LOCTEXT_NAMESPACE