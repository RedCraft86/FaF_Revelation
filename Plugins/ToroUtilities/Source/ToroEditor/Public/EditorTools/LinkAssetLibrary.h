// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCmds.h"
#include "ToroEdSettings.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

class FLinkAssetLibrary
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(LinkAssetLibrary, FLinkAssetLibrary);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(LinkAssetLibrary)
		REGISTER_TOOLBAR(LinkAssetLibrary, General)
	}

private:

	static void ExecuteAction()
	{
		const FString BasePath = UToroEdSettings::Get()->GetAssetLibraryPath();
		const FString TargetPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / FPaths::GetPathLeaf(BasePath));
		if (FPaths::DirectoryExists(TargetPath))
		{
			FNotificationInfo Info(FText::Format(
				INVTEXT("Failed to create file junction! Content folder \"{0}\" already exists."),
				FText::FromString(FPaths::GetPathLeaf(BasePath)))
			);
			Info.ExpireDuration = 2.0f;
			FSlateNotificationManager::Get().AddNotification(Info);
			return;
		}

		int32 RetCode;
		FString Output;
		const FString Command = FString::Printf(TEXT("cmd /c mklink /J \"%s\" \"%s\""), *TargetPath, *BasePath);
		FPlatformProcess::ExecProcess(TEXT("cmd.exe"), *Command, &RetCode, &Output, nullptr);
		if (RetCode == 0)
		{
			FUnrealEdMisc::Get().RestartEditor(false);
		}
		else
		{
			FNotificationInfo Info(FText::Format(
				INVTEXT("Failed to create file junction \"{0}.\""),
				FText::FromString(FPaths::GetPathLeaf(BasePath)))
			);
			Info.ExpireDuration = 2.0f;
			FSlateNotificationManager::Get().AddNotification(Info);
		}
	}
};
