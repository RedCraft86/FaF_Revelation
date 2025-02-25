// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"
#include "AssetToolsModule.h"
#include "BlueprintEditorModule.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"
#include "UnrealEd.h"

#include "ToroEditorCommands.h"
#include "EditorTools/LinkAssetLibrary.h"
#include "EditorTools/RestartEditor.h"
#include "EditorTools/StaticMeshBaker.h"
#include "EditorTools/StaticMeshMerger.h"
#include "EditorTools/StaticMeshInstancer.h"
#include "EditorTools/ChannelPacker.h"

#include "ToroMeshGenBase.h"
#include "NavPathVisualizer.h"
#include "LightProbes/LightProbe.h"
#include "Framework/MasterPostProcess.h"
#include "WorldActions/WorldActionActor.h"
#include "Misc/TeleportTargetActor.h"

#include "DetailsCustomization/ToroActorDetails.h"
#include "DetailsCustomization/LevelZoneVolumeDetails.h"
#include "DetailsCustomization/ElectricLightDetails.h"
#include "DetailsCustomization/PlayerFirstPersonDetails.h"
#include "DetailsCustomization/PlayerSideScrollerDetails.h"
#include "DetailsCustomization/PlayerThirdPersonDetails.h"

#include "DetailsCustomization/InventoryItemDetails.h"

#include "DetailsCustomization/InlineCurveDetails.h"
#include "DetailsCustomization/PrimitiveCollisionDetails.h"
#include "DetailsCustomization/InteractionInfoDetails.h"
#include "DetailsCustomization/ElectricLightAnimDetails.h"
#include "DetailsCustomization/LocalSoundEntryDetails.h"
#include "DetailsCustomization/InventoryMetadataDetails.h"
#include "DetailsCustomization/LockPlayerDetails.h"

#include "DetailsCustomization/ExpressiveTextFieldsDetails.h"
#include "DetailsCustomization/PropertyMetadataDetails.h"

#include "ComponentVisualizer/DebugShapeVisualizer.h"
#include "ComponentVisualizer/VisionConeVisualizer.h"

#include "AssetFactories/Inventory/InventoryItemAsset.h"
#include "AssetFactories/LocalSound/LocalSoundDatabaseAsset.h"
#include "AssetFactories/Achievement/AchievementDatabaseAsset.h"
#include "AssetFactories/GuidePage/GuidePageDatabaseAsset.h"

#define LOCTEXT_NAMESPACE "FToroEditorModule"

#define REGISTER_TOOL(Tool) Tool::Register(PluginCommands);
#define REGISTER_TOOL_MENUS(Tool) Tool::RegisterMenus(PluginCommands);

DEFINE_LOG_CATEGORY(LogToroEditor);

void FToroEditorModule::StartupModule()
{
	FToroEditorStyle::Init();

	// Toolbar Buttons
	{
		FToroEditorCommands::Register();

		PluginCommands = MakeShareable(new FUICommandList);
		REGISTER_TOOL(FLinkAssetLibrary)
		REGISTER_TOOL(FRestartEditor)
		REGISTER_TOOL(FStaticMeshBaker)
		REGISTER_TOOL(FStaticMeshMerger)
		REGISTER_TOOL(FStaticMeshInstancer)
		// REGISTER_TOOL(FChannelPacker)

		IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
		MainFrame.GetMainFrameCommandBindings()->Append(PluginCommands.ToSharedRef());
	
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FToroEditorModule::RegisterMenus));
	}

	// Blueprint Variable Metadata Editor
	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::LoadModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		BlueprintEditorModule->RegisterVariableCustomization(FProperty::StaticClass(),
			FOnGetVariableCustomizationInstance::CreateStatic(&FPropertyMetadataCustomization::MakeInstance));
	}

	// Struct and Class Details Customization
	if (FPropertyEditorModule* PropertyModule = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_CLASS_CUSTOMIZATION(AToroActor, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AToroVolume, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(ALightProbe, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AToroMeshGenBase, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(ANavPathVisualizer, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AMasterPostProcess, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AWorldActionActor, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(ATeleportTargetActor, FToroActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(ALevelZoneVolume, FLevelZoneVolumeCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AElectricActorBase, FElectricActorCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AElectricLightBase, FElectricLightCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AToroFirstPersonPlayer, FFirstPersonPlayerCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AToroThirdPersonPlayer, FThirdPersonPlayerCustomization)
		REGISTER_CLASS_CUSTOMIZATION(AToroSideScrollerPlayer, FSideScrollerPlayerCustomization)
		
		REGISTER_CLASS_CUSTOMIZATION(UInventoryItemData, FInventoryItemCustomization)
		
		REGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve, FInlineCurveCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve, FInlineCurveCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve, FInlineCurveCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision, FPrimitiveCollisionCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FInteractionInfo, FInteractionInfoCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FElectricLightAnim, FElectricLightAnimCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FLocalSoundEntry, FLocalSoundEntryCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FInventoryMetadata, FInventoryMetadataCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FPlayerLockFlag, FPlayerLockCustomization)
		
		REGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields, FExpressiveTextFieldsCustomization)

		// for (TObjectIterator<UScriptStruct> It; It; ++It)
		// {
		// 	const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
		// }
	}

	// Component Visualizers
	if (GUnrealEd)
	{
		REGISTER_VISUALIZER(UDebugShapeComponent, FDebugShapeVisualizer)
		REGISTER_VISUALIZER(UVisionConeComponent, FVisionConeVisualizer)
	}

	// Asset Type Actions
	if (const FAssetToolsModule* AssetToolsModule = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools"))
	{
		AssetTypeActions.Add(MakeShareable(new FInventoryItemAsset()));
		AssetTypeActions.Add(MakeShareable(new FLocalSoundDatabaseAsset()));
		AssetTypeActions.Add(MakeShareable(new FAchievementDatabaseAsset()));
		AssetTypeActions.Add(MakeShareable(new FGuidePageDatabaseAsset()));
		for (const TSharedPtr<IAssetTypeActions>& Action : AssetTypeActions)
		{
			AssetToolsModule->Get().RegisterAssetTypeActions(Action.ToSharedRef());
		}
	}
}

void FToroEditorModule::ShutdownModule()
{
	// Toolbar Buttons
	{
		UToolMenus::UnRegisterStartupCallback(this);
		UToolMenus::UnregisterOwner(this);
		FToroEditorCommands::Unregister();
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
		UNREGISTER_CLASS_CUSTOMIZATION(ALightProbe)
		UNREGISTER_CLASS_CUSTOMIZATION(AToroMeshGenBase)
		UNREGISTER_CLASS_CUSTOMIZATION(ANavPathVisualizer)
		UNREGISTER_CLASS_CUSTOMIZATION(AMasterPostProcess)
		UNREGISTER_CLASS_CUSTOMIZATION(AWorldActionActor)
		UNREGISTER_CLASS_CUSTOMIZATION(ATeleportTargetActor)
		UNREGISTER_CLASS_CUSTOMIZATION(ALevelZoneVolume)
		UNREGISTER_CLASS_CUSTOMIZATION(AElectricLightBase)
		UNREGISTER_CLASS_CUSTOMIZATION(AElectricActorBase)
		UNREGISTER_CLASS_CUSTOMIZATION(AToroFirstPersonPlayer)
		UNREGISTER_CLASS_CUSTOMIZATION(AToroThirdPersonPlayer)
		UNREGISTER_CLASS_CUSTOMIZATION(AToroSideScrollerPlayer)
		
		UNREGISTER_CLASS_CUSTOMIZATION(UInventoryItemData)
		
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInteractionInfo)
		UNREGISTER_STRUCT_CUSTOMIZATION(FElectricLightAnim)
		UNREGISTER_STRUCT_CUSTOMIZATION(FLocalSoundEntry)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInventoryMetadata)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPlayerLockFlag)
		
		UNREGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields)
		
		// for (TObjectIterator<UScriptStruct> It; It; ++It)
		// {
		// 	const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
		// }
	}

	// Component Visualizers
	if (GUnrealEd)
	{
		UNREGISTER_VISUALIZER(UDebugShapeComponent);
		UNREGISTER_VISUALIZER(UVisionConeComponent);
	}

	// Asset Type Actions
	if (const FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools"))
	{
		for (const TSharedPtr<IAssetTypeActions>& Action : AssetTypeActions)
		{
			if (Action.IsValid()) AssetToolsModule->Get().UnregisterAssetTypeActions(Action.ToSharedRef());
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
		REGISTER_TOOL_MENUS(FStaticMeshBaker)
		REGISTER_TOOL_MENUS(FStaticMeshMerger)
		REGISTER_TOOL_MENUS(FStaticMeshInstancer)
		// REGISTER_TOOL_MENUS(FChannelPacker)
	}
}

#define RootToContentDir StyleSet->RootToContentDir
TSharedPtr<FSlateStyleSet> FToroEditorStyle::StyleSet = nullptr;
void FToroEditorStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroEditorStyle"));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources/Icons"));

	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	
	StyleSet->Set("ToroEditor.LinkAssetLibrary", new IMAGE_BRUSH_SVG(TEXT("LinkFolder"), Icon20x20));
	StyleSet->Set("ToroEditor.RestartEditor", new IMAGE_BRUSH_SVG(TEXT("RestartEditor"), Icon20x20));
	StyleSet->Set("ToroEditor.ChannelPacker", new IMAGE_BRUSH_SVG(TEXT("ChannelPacker"), Icon20x20));
	StyleSet->Set("ToroEditor.StaticMeshBaker", new IMAGE_BRUSH_SVG(TEXT("StaticMeshBaker"), Icon20x20));
	StyleSet->Set("ToroEditor.StaticMeshMerger", new IMAGE_BRUSH_SVG(TEXT("StaticMeshMerger"), Icon20x20));
	StyleSet->Set("ToroEditor.StaticMeshInstancer", new IMAGE_BRUSH_SVG(TEXT("StaticMeshInstancer"), Icon20x20));
	StyleSet->Set("ToroEditor.InventoryGrid", new IMAGE_BRUSH_SVG(TEXT("InventoryGridSmall"), Icon20x20));
	
	StyleSet->Set("ClassThumbnail.InventoryItemData", new IMAGE_BRUSH_SVG(TEXT("InventoryGrid"), Icon64x64));
	StyleSet->Set("ClassThumbnail.LocalSoundDatabase", new IMAGE_BRUSH_SVG(TEXT("MusicChart"), Icon64x64));
	StyleSet->Set("ClassThumbnail.AchievementDatabase", new IMAGE_BRUSH_SVG(TEXT("Achievement"), Icon64x64));
	StyleSet->Set("ClassThumbnail.GuidePageDatabase", new IMAGE_BRUSH_SVG(TEXT("GuideBook"), Icon64x64));
	StyleSet->Set("ClassThumbnail.UDSSetterObject", new IMAGE_BRUSH_SVG(TEXT("UDS"), Icon64x64));
	StyleSet->Set("ClassThumbnail.LightProbe", new IMAGE_BRUSH(TEXT("LightProbe"), Icon64x64));
	StyleSet->Set("ClassThumbnail.MasterPostProcess", new IMAGE_BRUSH(TEXT("PostProcess"), Icon64x64));

	if (const TSharedPtr<IPlugin> ExpressiveText = IPluginManager::Get().FindPlugin(TEXT("ExpressiveText")))
	{
		StyleSet->Set("ClassIcon.ExprTextBlock", new FSlateImageBrush(
			ExpressiveText->GetContentDir() / TEXT("Core/Editor/Resources/ExTextLogo.png"),
			Icon16x16));
	}
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}
#undef RootToContentDir

void FToroEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)

#undef LOCTEXT_NAMESPACE