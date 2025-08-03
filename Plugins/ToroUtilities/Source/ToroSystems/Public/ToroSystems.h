// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "Helpers/ClassGetters.h"
#include "GameHints/HintDatabase.h"
#include "Tutorial/TutorialDatabase.h"
#include "ToroSystems.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroSystems, All, All);

class FToroSystemsModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Systems")
class TOROSYSTEMS_API UToroSysSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSysSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroSystems");

		LightProbePPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_LightProbe.PPM_LightProbe"));
	}

	SETTING_GETTER(UToroSysSettings)

	UPROPERTY(Config, EditAnywhere, Category = Systems)
		TSoftObjectPtr<UMaterialInterface> LightProbePPM;

	UPROPERTY(Config, EditAnywhere, Category = Systems)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

	UPROPERTY(Config, EditAnywhere, Category = Systems)
		TSoftObjectPtr<UTutorialDatabase> TutorialDB;

	UPROPERTY(Config, EditAnywhere, Category = Systems)
		TSoftObjectPtr<UHintDatabase> HintDB;
};
