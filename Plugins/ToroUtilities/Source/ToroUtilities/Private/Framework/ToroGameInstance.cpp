// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameInstance.h"
#include "UserSettings/ToroUserSettings.h"
#include "Libraries/ToroGeneralUtils.h"
#include "EnhancedCodeFlow.h"
#if UE_BUILD_SHIPPING
#include "GeneralProjectSettings.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#endif

void UToroGameInstance::OnWorldBeginPlay(UWorld* InWorld)
{
	if (UGameViewportClient* Viewport = InWorld ? InWorld->GetGameViewport() : nullptr)
	{
		Viewport->ViewModeIndex = CachedVMI;
	}

	if (!bRanFirstLoads)
	{
		bRanFirstLoads = true;
		UToroUserSettings::Get()->InitSettings(this);
		FFlow::Delay(this, 0.1f, [this]()
		{
			UToroGeneralUtils::RestartLevel(this);
		});
	}
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
