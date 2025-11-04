// Copyright (C) RedCraft86. All Rights Reserved.

#include "FaFRevEditor.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "DetailsPanel/PlayerCharacterDetails.h"
#include "Visualization/FVisionConeVisualizer.h"

#define LOCTEXT_NAMESPACE "FFaFRevEditorModule"

void FFaFRevEditorModule::StartupModule()
{
	if (GUnrealEd)
	{
		REGISTER_VISUALIZER(UVisionConeComponent, FVisionConeVisualizer)
	}
	
	if (FPropertyEditorModule* PropertyModule = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_CLASS_CUSTOMIZATION(APlayerCharacter, FPlayerCharacterDetails)
	}
}

void FFaFRevEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		UNREGISTER_VISUALIZER(UVisionConeComponent, FVisionConeVisualizer)
	}
	
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		UNREGISTER_CLASS_CUSTOMIZATION(APlayerCharacter)
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FFaFRevEditorModule, FaFRevEditor)