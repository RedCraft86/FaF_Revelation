// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCommands.h"
#include "ToroEditorSettings.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

class FAssetLibrary
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(AssetLibrary, FAssetLibrary);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(AssetLibrary)
		REGISTER_TOOLBAR(AssetLibrary, General)
	}

private:

	static void ExecuteAction();
	static void ShowErrorMsg(const FString& Reason, const FString& Path);
};