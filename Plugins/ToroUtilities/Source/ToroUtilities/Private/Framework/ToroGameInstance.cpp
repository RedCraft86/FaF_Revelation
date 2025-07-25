﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameInstance.h"
#include "Libraries/ToroGeneralUtils.h"
#include "UserSettings/ToroUserSettings.h"
#include "EnhancedCodeFlow.h"
#include "ToroUtilities.h"
#include "ToroConfigs.h"

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
		if (UToroUtilSettings::Get()->IsOnLaunchMap(this))
		{
			FFlow::Delay(this, 0.1f, [this]()
		   {
			   UToroGeneralUtils::RestartLevel(this);
		   });
		}
	}
}

void UToroGameInstance::SetUnlitViewmode(const bool bUnlit)
{
	const UWorld* World = GetWorld();
	if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
	{
		CachedVMI = bUnlit && GetConfigManager()->IsDeveloperMode() ? VMI_Unlit : VMI_Lit;
		Viewport->ViewModeIndex = CachedVMI;
	}
}

void UToroGameInstance::SetPlayerInvincible(const bool bInvincible)
{
	bPlayerInvincible = bInvincible && GetConfigManager()->IsDeveloperMode();
	OnPlayerInvincible.Broadcast(bPlayerInvincible);
}

UToroConfigManager* UToroGameInstance::GetConfigManager()
{
	if (ConfigManager) return ConfigManager;
	ConfigManager = GetSubsystem<UToroConfigManager>();
	return ConfigManager;
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
