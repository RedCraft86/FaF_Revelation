// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ClassGetterHelpers.h"
#include "GameSection/UDSTypes.h"
#include "Engine/DeveloperSettings.h"
#include "GameSection/GameSectionGraph.h"
#include "FaFRevSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "FaF Revelation")
class UFaFRevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFaFRevSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("FaFRev");
	}

	SETTING_GETTER_MUTABLE(UFaFRevSettings)

	UPROPERTY(Config, EditAnywhere, Category = GameSection)
		TSoftClassPtr<UUDSSetterObject> UDS_Setter;

	UPROPERTY(Config, EditAnywhere, Category = GameSection)
		TSoftObjectPtr<UGameSectionGraph> SectionGraph;
};
