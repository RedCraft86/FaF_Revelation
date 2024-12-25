// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroToolCommands.h"
#include "Framework/Commands/Commands.h"
#include "Subsystems/EditorActorSubsystem.h"

class FStaticMeshMerger
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(StaticMeshMerger, FStaticMeshMerger);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(StaticMeshMerger)
		REGISTER_TOOLBAR(StaticMeshMerger, Actor)
	}

private:

	static void ExecuteAction()
	{
		UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr;
		if (!Subsystem || (Subsystem && !Subsystem->GetSelectedLevelActors().IsEmpty()))
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FTabId("MergeActors"));
		}
	}
};