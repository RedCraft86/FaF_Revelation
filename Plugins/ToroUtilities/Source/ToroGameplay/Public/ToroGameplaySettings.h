// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroGameplaySettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Gameplay")
class TOROGAMEPLAY_API UToroGameplaySettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroGameplaySettings();
	
	SETTING_GETTER(UToroGameplaySettings)
};
