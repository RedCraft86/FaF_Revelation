﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "DataTypes/LocalSoundTypes.h"
#include "Styles/ExpressiveTextStyleBase.h"
#include "Materials/MaterialParameterCollection.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroRuntimeSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Runtime")
class TORORUNTIME_API UToroRuntimeSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroRuntimeSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroRuntime");

		LightProbeMPC = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/MPC_LightProbe.MPC_LightProbe"));
		LightProbePPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_LightProbe.PPM_LightProbe"));
		BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));
	}

	SETTING_GETTER_MUTABLE(UToroRuntimeSettings);
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<ULocalSoundDatabase> LocalSoundDatabase;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;

	UPROPERTY(Config, EditAnywhere, Category = LightProbes)
		TSoftObjectPtr<UMaterialParameterCollection> LightProbeMPC;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes)
		TSoftObjectPtr<UMaterialInterface> LightProbePPM;
	
	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<UMaterialInterface> BrightnessPPM;
	
	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<USoundMix> MainSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = UserSettings, meta = (ArraySizeEnum = "/Script/ToroRuntime.ESoundClassType"))
		TSoftObjectPtr<USoundClass> SoundClasses[static_cast<uint8>(ESoundClassType::MAX)];
};
