// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "Helpers/ClassGetters.h"
// #include "GamePhase/GamePhaseData.h"
#include "Engine/DeveloperSettings.h"
#include "FaFRevSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "FaF:Rev Settings")
class FAFREVGAME_API UFaFRevSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFaFRevSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("FaFRevSettings");
	}

	SETTING_GETTER(UFaFRevSettings)

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

	// UPROPERTY(Config, EditAnywhere, Category = Settings)
	// 	TSoftObjectPtr<UGamePhaseGraph> PhaseGraph;
};