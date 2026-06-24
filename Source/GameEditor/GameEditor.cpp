// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameEditor.h"

#include "DetailsPanel/StageTeleporterDetails.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"

#define LOCTEXT_NAMESPACE "GameEditor"

void FGameEditorModule::StartupModule()
{
	FDetailsCustomizationRegistry::Register<FStageTeleporter, FStageTeleporterDetails>();
}

void FGameEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGameEditorModule, GameEditor)