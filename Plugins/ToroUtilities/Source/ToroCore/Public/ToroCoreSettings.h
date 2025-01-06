// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "Styles/ExpressiveTextStyleBase.h"
#include "ToroCoreSettings.generated.h"

UCLASS(Config = Engine, DefaultConfig, DisplayName = "Toro Utilities")
class TOROCORE_API UToroCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroCoreSettings()
#if WITH_EDITOR
		: StartupCommands({
			{ TEXT("r.VSyncEditor"), TEXT("1") },
			{ TEXT("r.Streaming.PoolSize"), TEXT("3000") }
		})
#endif
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");
	}

	SETTING_GETTER(UToroCoreSettings)
	
	UPROPERTY(Config, EditAnywhere, Category = "Core")
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Config, EditAnywhere, Category = "Editor")
		TMap<FString, FString> StartupCommands;
#endif
};