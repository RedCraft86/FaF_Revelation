﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterHelpers.h"
#include "Styles/ExpressiveTextStyleBase.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Runtime Settings")
class TORORUNTIME_API UToroSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings()
	{
		CategoryName = TEXT("ToroUtilities");
		SectionName = TEXT("RuntimeSettings");
	}

	SETTING_GETTER(UToroSettings)
	
	UPROPERTY(Config, EditAnywhere, Category = "Settings")
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;
};