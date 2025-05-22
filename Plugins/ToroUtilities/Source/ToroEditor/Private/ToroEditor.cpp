// Copyright (C) RedCraft86, Inc. All Rights Reserved.

#include "ToroEditor.h"
#include "UnrealEdGlobals.h"
#include "BlueprintEditorModule.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IMainFrameModule.h"
#include "Interfaces/IPluginManager.h"

#include "ToroCmds.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define REGISTER_TOOL(Tool) Tool::Register(PluginCommands);
#define REGISTER_TOOL_MENUS(Tool) Tool::RegisterMenus(PluginCommands);

#define LOCTEXT_NAMESPACE "FToroEditorModule"

void FToroEditorModule::StartupModule()
{
	FToroEditorStyle::Init();

	// Toolbar Buttons
	{
		FToroCmds::Register();
		PluginCommands = MakeShareable(new FUICommandList);
		// REGISTER_TOOL(FLinkAssetLibrary)
		// REGISTER_TOOL(FRestartEditor)
		// REGISTER_TOOL(FActorLayout)
		// REGISTER_TOOL(FStaticMeshBaker)
		// REGISTER_TOOL(FStaticMeshMerger)
		// REGISTER_TOOL(FStaticMeshInstancer)

		IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
		MainFrame.GetMainFrameCommandBindings()->Append(PluginCommands.ToSharedRef());
	
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FToroEditorModule::RegisterMenus));
	}

	// Blueprint Variable Metadata Editor
	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::LoadModulePtr<FBlueprintEditorModule>("Kismet"))
	{
	}

	// Struct and Class Details Customization
	if (FPropertyEditorModule* PropertyModule = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
		}
	}

	// Component Visualizers
	if (GUnrealEd)
	{
	}
}

void FToroEditorModule::ShutdownModule()
{
	// Toolbar Buttons
	{
		UToolMenus::UnRegisterStartupCallback(this);
		UToolMenus::UnregisterOwner(this);
		FToroCmds::Unregister();
	}

	// Blueprint Variable Metadata Editor
	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::GetModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		const FDelegateHandle Handle;
		BlueprintEditorModule->UnregisterVariableCustomization(FProperty::StaticClass(), Handle);
	}

	// Struct and Class Details Customization
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
		}
	}

	// Component Visualizers
	if (GUnrealEd)
	{
	}

	FToroEditorStyle::Shutdown();
}

void FToroEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
	}
}

#define RootToContentDir StyleSet->RootToContentDir
TSharedPtr<FSlateStyleSet> FToroEditorStyle::StyleSet = nullptr;
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

	StyleSet->Set("ClassThumbnail.FootstepDatabase", new IMAGE_BRUSH_SVG(TEXT("Footstep"), Icon64x64));
	StyleSet->Set("ClassThumbnail.GuidePageDatabase", new IMAGE_BRUSH_SVG(TEXT("GuideBook"), Icon64x64));
	StyleSet->Set("ClassThumbnail.AchievementDatabase", new IMAGE_BRUSH_SVG(TEXT("Achievement"), Icon64x64));
	StyleSet->Set("ClassThumbnail.LocalSoundDatabase", new IMAGE_BRUSH_SVG(TEXT("MusicChart"), Icon64x64));
	StyleSet->Set("ClassThumbnail.MasterPostProcess", new IMAGE_BRUSH(TEXT("PostProcess"), Icon64x64));
	StyleSet->Set("ClassThumbnail.LightProbe", new IMAGE_BRUSH(TEXT("LightProbe"), Icon64x64));

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