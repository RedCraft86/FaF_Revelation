// Copyright (C) RedCraft86. All Rights Reserved.

#include "FaF_RevelationEditor.h"
#include "Details/PlayerDetails.h"
#include "Details/PlayerFootstepsDetails.h"

#define LOCTEXT_NAMESPACE "FFaF_RevelationEditorModule"

void FFaF_RevelationEditorModule::StartupModule()
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_CLASS_CUSTOMIZATION(AGamePlayerBase, FPlayerDetailsCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FPlayerFootsteps, FPlayerFootstepCustomization)
	}
}

void FFaF_RevelationEditorModule::ShutdownModule()
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		UNREGISTER_CLASS_CUSTOMIZATION(AGamePlayerBase)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPlayerFootsteps)
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FFaF_RevelationEditorModule, FaF_RevelationEditor)