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

	SETTING_GETTER(UFaFRevSettings)

	UFaFRevSettings(): EnemyToThemeState({
		{EEnemyState::None,			0},
		{EEnemyState::Roaming,			1},
		{EEnemyState::Investigating,	2},
		{EEnemyState::Stalking,		3},
		{EEnemyState::Chasing,			4},
		{EEnemyState::Searching,		5},
	})
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("FaFRevSettings");
	}

	UPROPERTY(Config, EditAnywhere, Category = Game, meta = (ReadOnlyKeys = true))
		TMap<EEnemyState, uint8> EnemyToThemeState;
};
