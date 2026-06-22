// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameEditor.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"
#include "DetailsPanel/GameStageNodeDetails.h"

#define LOCTEXT_NAMESPACE "FGameEditorModule"

void FGameEditorModule::StartupModule()
{
	if (FPropertyEditorModule* Module = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		Module->RegisterCustomClassLayout(UGameStageNode::StaticClass()->GetFName(), 
			FOnGetDetailCustomizationInstance::CreateStatic(&FGameStageNodeDetails::MakeInstance));
	}
}

void FGameEditorModule::ShutdownModule()
{
	if (FPropertyEditorModule* Module = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		Module->UnregisterCustomClassLayout(UGameStageNode::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGameEditorModule, GameEditor)