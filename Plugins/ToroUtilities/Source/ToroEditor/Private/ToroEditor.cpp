// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"
#include "UnrealEd.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#include "ComponentVis/EditorShapeVisualizer.h"
#include "ComponentVis/VisionConeVisualizer.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define LOCTEXT_NAMESPACE "ToroEditor"

void FToroEditorModule::StartupModule()
{
	FToroEditorStyle::Init();
	// Component Visualizers
	if (GUnrealEd)
	{
		REGISTER_VISUALIZER(UEditorShapeComponent, FEditorShapeVisualizer)
		REGISTER_VISUALIZER(UVisionConeComponent, FVisionConeVisualizer)
	}
}

void FToroEditorModule::ShutdownModule()
{
	// Component Visualizers
	if (GUnrealEd)
	{
		UNREGISTER_VISUALIZER(UEditorShapeComponent)
		UNREGISTER_VISUALIZER(UVisionConeComponent)
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
#define AddSVG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH_SVG(TEXT(#ImageFile), Icon##Size));
#define AddPNG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH(TEXT(#ImageFile), Icon##Size));
TSharedPtr<FSlateStyleSet> FToroEditorStyle::StyleSet = nullptr;
void FToroEditorStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroEdStyle"));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(
		TEXT("ToroUtilities"))->GetBaseDir() / TEXT("Resources"));

	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);

	AddSVG(ToroEditor.LinkAssetLibrary, LinkFolder, 20x20);
	AddSVG(ToroEditor.RestartEditor, RestartEditor, 20x20);
	AddSVG(ToroEditor.ActorLayout, ActorLayout, 20x20);
	AddSVG(ToroEditor.StaticMeshMerger, StaticMeshMerger, 20x20);
	AddSVG(ToroEditor.StaticMeshBaker, StaticMeshBaker, 20x20);
	AddSVG(ToroEditor.StaticMeshInstancer, StaticMeshInstancer, 20x20);

	AddSVG(ClassThumbnail.SplineCable, SplineCable, 64x64);
	AddSVG(ClassThumbnail.SplineBarrier, SplineBarrier, 64x64);
	AddPNG(ClassThumbnail.MasterPostProcess, MasterPostProcess, 64x64);
	AddPNG(ClassThumbnail.LightProbeActor, LightProbe, 64x64);

	AddSVG(ClassThumbnail.InventoryAsset, InventoryGrid, 64x64);
	AddSVG(ClassThumbnail.AchievementDatabase, Achievements, 64x64);
	AddSVG(ClassThumbnail.TutorialDatabase, Tutorial, 64x64);
	
	AddSVG(ToroEditor.InventoryGridSmall, InventoryGridSmall, 32x32);

	if (const TSharedPtr<IPlugin> ETPlugin = IPluginManager::Get().FindPlugin(TEXT("ExpressiveText")))
	{
		StyleSet->Set("ClassIcon.ExprTextBlock", new FSlateImageBrush(
			ETPlugin->GetContentDir() / TEXT("Core/Editor/Resources/ExTextLogo.png"),
			Icon16x16
		));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}
#undef AddPNG
#undef AddSVG
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

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)