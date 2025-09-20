// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroFrameworkSettings.generated.h"

UCLASS(Config = Engine, DefaultConfig, DisplayName = "Toro Framework")
class TOROFRAMEWORK_API UToroFrameworkSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroFrameworkSettings();
	
	SETTING_GETTER(UToroFrameworkSettings)
};
