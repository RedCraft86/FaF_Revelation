// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "Styles/ExpressiveTextStyleBase.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Utilities")
class TOROCORE_API UToroSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");
	}

	SETTING_GETTER(UToroSettings)
	
	UPROPERTY(Config, EditAnywhere, Category = "Runtime")
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;
};