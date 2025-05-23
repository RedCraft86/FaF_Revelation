// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "Engine/DeveloperSettings.h"
#include "Framework/ToroPlayerController.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Utilities")
class TOROUTILITIES_API UToroSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");
	}

	SETTING_GETTER_MUTABLE(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UInputMappingContext> DefaultInputMappings;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSet<TSoftClassPtr<UUserWidget>> DefaultWidgets;
};
