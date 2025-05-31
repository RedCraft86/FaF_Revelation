// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "Engine/DeveloperSettings.h"
#include "GameSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Game Settings")
class GAMERUNTIME_API UGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UGameSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("GameSettings");
	}

	SETTING_GETTER_MUTABLE(UGameSettings)

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftClassPtr<UUserWidget> NarrativeWidget;
};