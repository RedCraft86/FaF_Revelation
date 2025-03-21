// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameEditor.h"
#include "ToroEditor.h"

#include "DetailsCustomization/GamePlayerDetails.h"
#include "DetailsCustomization/PlayerFootstepDetails.h"

#define LOCTEXT_NAMESPACE "FGameEditorModule"

void FGameEditorModule::StartupModule()
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_CLASS_CUSTOMIZATION(AGamePlayerBase, FPlayerDetailsCustomization)
		REGISTER_STRUCT_CUSTOMIZATION(FPlayerFootsteps, FPlayerFootstepCustomization)
	}
}

void FGameEditorModule::ShutdownModule()
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		UNREGISTER_CLASS_CUSTOMIZATION(AGamePlayerBase)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPlayerFootsteps)
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGameEditorModule, GameEditor)