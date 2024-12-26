// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"
#include "UnrealEdGlobals.h"
#include "AssetToolsModule.h"
#include "BlueprintEditorModule.h"
#include "DataTypes/PrimitiveData.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"

#include "DetailsCustomization/PropertyMetadataDetails.h"
#include "DetailsCustomization/PrimitiveCollisionDetails.h"
#include "DetailsCustomization/InlineCurveDetails.h"

#define LOCTEXT_NAMESPACE "FToroEditorModule"

void FToroEditorModule::StartupModule()
{
	FToroEditorStyle::Init();

	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::LoadModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		BlueprintEditorModule->RegisterVariableCustomization(FProperty::StaticClass(),
			FOnGetVariableCustomizationInstance::CreateStatic(&FPropertyMetadataCustomization::MakeInstance));
	}

	if (FPropertyEditorModule* PropertyModule = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve, FInlineCurveCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve, FInlineCurveCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve, FInlineCurveCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision, FPrimitiveCollisionCustomization)

		// for (TObjectIterator<UScriptStruct> It; It; ++It)
		// {
		// 	const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
		// }
	}

	if (GUnrealEd)
	{
	}

	if (const FAssetToolsModule* AssetToolsModule = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools"))
	{
		for (const TSharedPtr<IAssetTypeActions>& Action : AssetTypeActions)
		{
			AssetToolsModule->Get().RegisterAssetTypeActions(Action.ToSharedRef());
		}
	}
}

void FToroEditorModule::ShutdownModule()
{
	FToroEditorStyle::Shutdown();

	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::GetModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		const FDelegateHandle Handle;
		BlueprintEditorModule->UnregisterVariableCustomization(FProperty::StaticClass(), Handle);
	}

	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision)
		
		// for (TObjectIterator<UScriptStruct> It; It; ++It)
		// {
		// 	const UScriptStruct* ScriptStruct = *It; if (!ScriptStruct) continue;
		// }
	}

	
	if (GUnrealEd)
	{
	}

	if (const FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools"))
	{
		for (const TSharedPtr<IAssetTypeActions>& Action : AssetTypeActions)
		{
			if (Action.IsValid()) AssetToolsModule->Get().UnregisterAssetTypeActions(Action.ToSharedRef());
		}
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
	StyleSet->Set("SmallIcon.InventoryGrid", new IMAGE_BRUSH_SVG(TEXT("InventoryGridSmall"), Icon20x20));
	StyleSet->Set("ClassThumbnail.InventoryItemBase", new IMAGE_BRUSH_SVG(TEXT("InventoryGrid"), Icon64x64));
	StyleSet->Set("ClassThumbnail.MusicDataBase", new IMAGE_BRUSH_SVG(TEXT("MusicData"), Icon64x64));
	
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
#undef RootToContentDir
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)

#undef LOCTEXT_NAMESPACE