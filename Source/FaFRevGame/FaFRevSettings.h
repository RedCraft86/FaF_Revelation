// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "FaFRevSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "FaFRev Settings")
class FAFREVGAME_API UFaFRevSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	SETTING_GETTER(UFaFRevSettings)

	UFaFRevSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("FaFRevSettings");
	}
};
