// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"
#include "UnrealEdGlobals.h"
#include "BlueprintEditorModule.h"
#include "Editor/UnrealEdEngine.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"

#include "ToroCmds.h"
#include "Toolbar/ActorLayout.h"
#include "Toolbar/LinkAssetLibrary.h"
#include "Toolbar/RestartEditor.h"
#include "Toolbar/StaticMeshBaker.h"
#include "Toolbar/StaticMeshInstancer.h"
#include "Toolbar/StaticMeshMerger.h"

#include "ComponentVis/EditorShapesVisualizer.h"

#include "DetailsPanels/PropertyMetadataDetails.h"

#include "DetailsPanels/ToroActorDetails.h"
#include "DetailsPanels/Class/ZoneVolumeDetails.h"
#include "DetailsPanels/Struct/InlineCurveDetails.h"
#include "DetailsPanels/Struct/PrimitiveCollisionDetails.h"
#include "DetailsPanels/Struct/ExprTextFieldsDetails.h"

#include "Actors/TeleportPoint.h"
#include "Actors/NavPathVisualizer.h"
#include "Procedural/ToroProcGenBase.h"
#include "LightProbes/LightProbeActor.h"
#include "MiscActors/MasterPostProcess.h"

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
		REGISTER_VISUALIZER(UEditorShapesComponent, FEditorShapesVisualizer)
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
		REGISTER_CLASS_CUSTOMIZATION(ATeleportPoint, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(ANavPathVisualizer, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(AToroProcGenBase, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(AMasterPostProcess, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(ALightProbeActor, FToroActorDetails)
		REGISTER_CLASS_CUSTOMIZATION(ALevelZoneVolume, FZoneVolumeDetails)

		REGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision, FPrimitiveCollisionDetails)

		REGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields, FExprTextFieldsDetails)

		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
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
		UNREGISTER_VISUALIZER(UEditorShapesComponent)
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
		UNREGISTER_CLASS_CUSTOMIZATION(AToroCharacter)
		UNREGISTER_CLASS_CUSTOMIZATION(ATeleportPoint)
		UNREGISTER_CLASS_CUSTOMIZATION(ANavPathVisualizer)
		UNREGISTER_CLASS_CUSTOMIZATION(AToroProcGenBase)
		UNREGISTER_CLASS_CUSTOMIZATION(AMasterPostProcess)
		UNREGISTER_CLASS_CUSTOMIZATION(ALightProbeActor)
		UNREGISTER_CLASS_CUSTOMIZATION(ALevelZoneVolume)

		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision)

		UNREGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields)

		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
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
#define AddSVG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH_SVG(TEXT(#ImageFile), Icon##Size));
#define AddPNG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH(TEXT(#ImageFile), Icon##Size));
void FToroEditorStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroEditorStyle"));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(
		TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources"));

	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);

	AddSVG(ToroEditor.LinkAssetLibrary, LinkFolder, 20x20);
	AddSVG(ToroEditor.RestartEditor, RestartEditor, 20x20);
	AddSVG(ToroEditor.ActorLayout, ActorLayout, 20x20);
	AddSVG(ToroEditor.StaticMeshBaker, StaticMeshBaker, 20x20);
	AddSVG(ToroEditor.StaticMeshMerger, StaticMeshMerger, 20x20);
	AddSVG(ToroEditor.StaticMeshInstancer, StaticMeshInstancer, 20x20);
	AddSVG(ToroEditor.InventoryGridSmall, InventoryGridSmall, 32x32);

	AddSVG(ClassThumbnail.SplineCable, SplineCable, 64x64);
	AddSVG(ClassThumbnail.SplineBarrier, SplineBarrier, 64x64);
	AddPNG(ClassThumbnail.MasterPostProcess, PostProcess, 64x64);
	AddPNG(ClassThumbnail.LightProbeActor, LightProbe, 64x64);

	AddSVG(ClassThumbnail.TutorialDatabase, GuideBook, 64x64);
	AddSVG(ClassThumbnail.InventoryAsset, InventoryGrid, 64x64);
	AddSVG(ClassThumbnail.AchievementDatabase, Achievement, 64x64);

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