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

	UFaFRevSettings(): LowThreatRadius(500.0f), ThemeRefreshRate(0.5f)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("FaFRevSettings");
	}

	SETTING_GETTER(UFaFRevSettings)

	UPROPERTY(Config, EditAnywhere, Category = EnemyTheme, meta = (ClampMin = 200.0f, UIMin = 200.0f))
		float LowThreatRadius;

	UPROPERTY(Config, EditAnywhere, Category = EnemyTheme, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float ThemeRefreshRate;
};
