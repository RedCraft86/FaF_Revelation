// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditorCommands.h"
#include "Framework/Commands/Commands.h"

class FFMODOpener
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(FMODOpener, FFMODOpener);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(FMODOpener)
		REGISTER_TOOLBAR(FMODOpener, General)
	}

private:

	static void ExecuteAction()
	{
		const FString FMODPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("RunFMOD.bat"));
		FPlatformProcess::CreateProc(*FMODPath, FApp::GetProjectName(),
			true, false, false, nullptr,
			0, nullptr, nullptr);
	}
};
