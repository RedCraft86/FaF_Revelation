﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameInstance.h"
#if UE_BUILD_SHIPPING
#include "GeneralProjectSettings.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#endif

void UToroGameInstance::OnWorldBeginPlay(UWorld* InWorld)
{
	WorldContext = InWorld;
}

void UToroGameInstance::SetUnlitViewmode(const bool bUnlit)
{
	const UWorld* World = GetWorld();
	if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
	{
		CachedVMI = bUnlit ? VMI_Unlit : VMI_Lit;
		Viewport->ViewModeIndex = CachedVMI;
	}
}

void UToroGameInstance::Init()
{
	Super::Init();
#if UE_BUILD_SHIPPING
	// Only one instance of the game can be initialized!
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		if (!IFileManager::Get().CreateFileWriter(*(FPaths::ProjectSavedDir() / ProjectSettings->ProjectID.ToString()), 0))
		{
			FPlatformApplicationMisc::RequestMinimize();
			FPlatformMisc::RequestExit(false);
		}
	}
#endif
}
