// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameInstance.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "GeneralProjectSettings.h"
#include "Helpers/WindowsHelpers.h"

void UToroGameInstance::OnWorldBeginPlay(UWorld* InWorld)
{
	// TODO: Run first loads
}

void UToroGameInstance::Init()
{
	Super::Init();
#if UE_BUILD_SHIPPING
	// Only allow one game instance to run
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		if (!IFileManager::Get().CreateFileWriter(*(FPaths::ProjectSavedDir() / ProjectSettings->ProjectID.ToString()), 0))
		{
			WindowsHelpers::OpenDialogue(TEXT("Failed to Launch Game"),
				TEXT("An instance of this game is already running!"),
				EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);

			FPlatformApplicationMisc::RequestMinimize();
			FPlatformMisc::RequestExit(false);
		}
	}
#endif
}
