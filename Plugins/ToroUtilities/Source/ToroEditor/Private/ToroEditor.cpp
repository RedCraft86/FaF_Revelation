// Copyright (C) RedCraft86, Inc. All Rights Reserved.

#include "ToroEditor.h"
#include "UnrealEdGlobals.h"
#include "BlueprintEditorModule.h"
#include "Editor/UnrealEdEngine.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IMainFrameModule.h"
#include "Interfaces/IPluginManager.h"

#include "ToroCmds.h"
#include "EditorTools/ActorLayout.h"
#include "EditorTools/LinkAssetLibrary.h"
#include "EditorTools/RestartEditor.h"
#include "EditorTools/StaticMeshBaker.h"
#include "EditorTools/StaticMeshInstancer.h"
#include "EditorTools/StaticMeshMerger.h"

#include "ComponentVis/WireShapeVisualizer.h"

#include "DetailsPanels/PropertyMetadataDetails.h"
#include "DetailsPanels/ToroActorDetails.h"

#include "DetailsPanels/Struct/FloatModifierDetails.h"
#include "DetailsPanels/Struct/InlineCurveDetails.h"
#include "DetailsPanels/Struct/PrimitiveCollisionDetails.h"
#include "DetailsPanels/Struct/ExprTextFieldsDetails.h"

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
		REGISTER_TOOL(FLinkAssetLibrary)
		REGISTER_TOOL(FRestartEditor)
		REGISTER_TOOL(FActorLayout)
		REGISTER_TOOL(FStaticMeshBaker)
		REGISTER_TOOL(FStaticMeshMerger)
		REGISTER_TOOL(FStaticMeshInstancer)

		IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
		MainFrame.GetMainFrameCommandBindings()->Append(PluginCommands.ToSharedRef());
	
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FToroEditorModule::RegisterMenus));
	}

	// Component Visualizers
	if (GUnrealEd)
	{
		REGISTER_VISUALIZER(UWireShapeComponent, FWireShapeVisualizer)
	}

	// Blueprint Variable Metadata Editor
	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::LoadModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		BlueprintEditorModule->RegisterVariableCustomization(FProperty::StaticClass(),
			FOnGetVariableCustomizationInstance::CreateStatic(&FPropertyMetadataDetails::MakeInstance));
	}

	// Struct and Class Details Customization
	if (FPropertyEditorModule* PropertyModule = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_CLASS_CUSTOMIZATION(AToroActor, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(AToroVolume, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(AToroCharacter, FToroActorDetails)
		
		REGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision, FPrimitiveCollisionDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields, FExprTextFieldsDetails)
		
		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
			REGISTER_STRUCT_CUSTOMIZATION_INHERITED(FToroFloatModifier, FFloatModifierDetails)
		}
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

	// Component Visualizers
	if (GUnrealEd)
	{
		UNREGISTER_VISUALIZER(UWireShapeComponent)
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
		UNREGISTER_CLASS_CUSTOMIZATION(AToroActor)
		UNREGISTER_CLASS_CUSTOMIZATION(AToroVolume)
		
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision)
		UNREGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields)
		
		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
			UNREGISTER_STRUCT_CUSTOMIZATION_INHERITED(FToroFloatModifier)
		}
	}

	FToroEditorStyle::Shutdown();
}

void FToroEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		REGISTER_TOOL_MENUS(FLinkAssetLibrary)
		REGISTER_TOOL_MENUS(FRestartEditor)
		REGISTER_TOOL_MENUS(FActorLayout)
		REGISTER_TOOL_MENUS(FStaticMeshBaker)
		REGISTER_TOOL_MENUS(FStaticMeshMerger)
		REGISTER_TOOL_MENUS(FStaticMeshInstancer)
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