// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroRuntimeSettings.generated.h"

UCLASS(Config = Engine, DefaultConfig, DisplayName = "Toro Runtime")
class TORORUNTIME_API UToroRuntimeSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroRuntimeSettings();
	SETTING_GETTER(UToroRuntimeSettings)
};
