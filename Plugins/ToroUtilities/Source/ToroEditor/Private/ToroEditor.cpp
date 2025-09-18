// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"
#include "UnrealEd.h"

#include "ComponentVis/EditorShapeVisualizer.h"
#include "ComponentVis/VisionConeVisualizer.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define LOCTEXT_NAMESPACE "ToroEditor"

void FToroEditorModule::StartupModule()
{
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
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)