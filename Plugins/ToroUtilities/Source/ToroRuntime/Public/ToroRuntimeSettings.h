// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "DataTypes/LocalSoundTypes.h"
#include "Styles/ExpressiveTextStyleBase.h"
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

		LightProbePPM_8 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPMI_LightProbe_8.PPMI_LightProbe_8"));
		LightProbePPM_16 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPMI_LightProbe_16.PPMI_LightProbe_16"));
		LightProbePPM_24 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPMI_LightProbe_24.PPMI_LightProbe_24"));
		LightProbePPM_32 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPMI_LightProbe_32.PPMI_LightProbe_32"));
		BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));
	}

	SETTING_GETTER_MUTABLE(UToroRuntimeSettings)
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<ULocalSoundDatabase> LocalSoundDatabase;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Post Process (Max 8 Probes)")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_8;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Post Process (Max 16 Probes)")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_16;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Post Process (Max 24 Probes)")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_24;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Post Process (Max 32 Probes)")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_32;
	
	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<UMaterialInterface> BrightnessPPM;
	
	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<USoundMix> MainSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = UserSettings, meta = (ArraySizeEnum = "/Script/ToroRuntime.ESoundClassType"))
		TSoftObjectPtr<USoundClass> SoundClasses[static_cast<uint8>(ESoundClassType::MAX)];

	UMaterialInterface* GetProbeMaterial(const uint8 NumProbes) const
	{
		if (NumProbes <= 8) return LightProbePPM_8.LoadSynchronous();
		if (NumProbes <= 16) return LightProbePPM_16.LoadSynchronous();
		if (NumProbes <= 24) return LightProbePPM_24.LoadSynchronous();
		return LightProbePPM_32.LoadSynchronous();
	}
};
