// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "Characters/Enemy/GameEnemyBase.h"
#include "FaFRevSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "FaFRev Settings")
class FAFREVGAME_API UFaFRevSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFaFRevSettings(): EnemyThemeRadius(500.0f), EnemyThemeUpdateInterval(0.5f)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("FaFRevSettings");
	}

	SETTING_GETTER(UFaFRevSettings)

	UPROPERTY(Config, EditAnywhere, Category = EnemyTheme, DisplayName = "Radius", meta = (ClampMin = 200.0f, UIMin = 200.0f))
		float EnemyThemeRadius;

	UPROPERTY(Config, EditAnywhere, Category = EnemyTheme, DisplayName = "Update Interval", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float EnemyThemeUpdateInterval;
};
