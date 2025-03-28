﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameInstance.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroGeneralUtils.h"
#include "EnhancedCodeFlow.h"
#if UE_BUILD_SHIPPING
#include "GeneralProjectSettings.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#endif

EToroValidPins UToroGameInstance::GetToroGameInstance(UToroGameInstance*& OutObject,
	const UObject* ContextObject, const TSubclassOf<UToroGameInstance>& Class)
{
	UToroGameInstance* Obj = Get<UToroGameInstance>(ContextObject);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void UToroGameInstance::OnWorldBeginPlay(UWorld* InWorld)
{
	if (UGameViewportClient* Viewport = InWorld ? InWorld->GetGameViewport() : nullptr)
	{
		Viewport->ViewModeIndex = CachedVMI;
	}

	if (!bFirstLoads)
	{
		bFirstLoads = true;
		UToroUserSettings::Get()->InitSettings();
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

void UToroGameInstance::SetPlayerInvisible(const bool bInvisible)
{
	bPlayerInvisible = bInvisible;
	OnPlayerInvisible.Broadcast(bInvisible);
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
