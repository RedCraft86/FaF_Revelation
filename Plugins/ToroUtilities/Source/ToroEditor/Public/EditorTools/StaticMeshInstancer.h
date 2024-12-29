// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditorCommands.h"
#include "Framework/Commands/Commands.h"

class FStaticMeshInstancer
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(StaticMeshInstancer, FStaticMeshInstancer);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(StaticMeshBaker)
		REGISTER_TOOLBAR(StaticMeshInstancer, Actor)
	}

private:

	static void ExecuteAction();
};