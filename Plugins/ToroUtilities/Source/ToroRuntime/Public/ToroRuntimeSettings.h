﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "InputMappingContext.h"
#include "Engine/DeveloperSettings.h"
#include "GuidePages/GuidePageTypes.h"
#include "SaveSystem/BaseSaveObjects.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "MusicSystem/LocalSoundTypes.h"
#include "Achievements/AchievementTypes.h"
#include "Styles/ExpressiveTextStyleBase.h"
#include "UserSettings/ToroUserSettings.h"
#include "ToroRuntimeSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Runtime")
class TORORUNTIME_API UToroRuntimeSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroRuntimeSettings() : WordsPerSecond(2)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroRuntime");

		DefaultTextStyle = FSoftObjectPath(TEXT("/ToroUtilities/Classes/DefaultExprTextStyle.DefaultExprTextStyle"));

		SaveObjects = {
			{Tag_GlobalSave, UGlobalSaveObjectBase::StaticClass()},
			{Tag_GameSave, UGameSaveObjectBase::StaticClass()},
		};

		bUseLightProbes = true;
		LightProbePPM_8 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/LightProbe/PPMI_LP_8.PPMI_LP_8"));
		LightProbePPM_16 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/LightProbe/PPMI_LP_16.PPMI_LP_16"));
		LightProbePPM_24 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/LightProbe/PPMI_LP_24.PPMI_LP_24"));
		LightProbePPM_32 = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/LightProbe/PPMI_LP_32.PPMI_LP_32"));
		BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));

		MainSoundMix = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/SMC_Master.SMC_Master"));
		SoundClasses[0] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/SC_Master.SC_Master"));
		SoundClasses[1] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/SC_Music.SC_Music"));
		SoundClasses[2] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/SC_SoundFX.SC_SoundFX"));
		SoundClasses[3] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/SC_Voice.SC_Voice"));
	}

	SETTING_GETTER_MUTABLE(UToroRuntimeSettings)
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UAchievementDatabase> AchievementDatabase;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<ULocalSoundDatabase> LocalSoundDatabase;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UGuidePageDatabase> GuidePageDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UInputMappingContext> DefaultInputMappings;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;
	
	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSet<TSoftClassPtr<UToroWidget>> DefaultWidgets;

	UPROPERTY(Config, EditAnywhere, Category = SaveSystem)
		FName DemoVersion;
	
	UPROPERTY(Config, EditAnywhere, Category = SaveSystem, meta = (ForceInlineRow, Categories = "Saves"))
		TMap<FGameplayTag, TSoftClassPtr<UToroSaveObject>> SaveObjects;

	UPROPERTY(Config, EditAnywhere, Category = LightProbes)
		bool bUseLightProbes;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Max 8 Probes PPM")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_8;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Max 16 Probes PPM")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_16;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Max 24 Probes PPM")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_24;
	
	UPROPERTY(Config, EditAnywhere, Category = LightProbes, DisplayName = "Max 32 Probes PPM")
		TSoftObjectPtr<UMaterialInterface> LightProbePPM_32;
	
	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<UMaterialInterface> BrightnessPPM;
	
	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<USoundMix> MainSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = UserSettings, meta = (ArraySizeEnum = "/Script/ToroRuntime.ESoundClassType"))
		TSoftObjectPtr<USoundClass> SoundClasses[static_cast<uint8>(ESoundClassType::MAX)];

	float GetReadingTime(const FString& InString) const
	{
		TArray<FString> Words;
		InString.ParseIntoArray(Words, TEXT(" "));
		return Words.Num() / FMath::Max(1.0f, (float)WordsPerSecond);
	}
	
	bool IsOnGameplayMap(const UObject* WorldContext) const
	{
		if (GameplayMap.IsNull()) return false;
		return UGameplayStatics::GetCurrentLevelName(WorldContext) == GameplayMap.GetAssetName();
	}
	
	bool IsUsingLightProbes() const
	{
		return bUseLightProbes && !LightProbePPM_8.IsNull() && !LightProbePPM_16.IsNull()
			&& !LightProbePPM_24.IsNull() && !LightProbePPM_32.IsNull();
	}
	
	UMaterialInterface* GetProbeMaterial(const uint8 NumProbes) const
	{
		if (NumProbes <= 8) return LightProbePPM_8.LoadSynchronous();
		if (NumProbes <= 16) return LightProbePPM_16.LoadSynchronous();
		if (NumProbes <= 24) return LightProbePPM_24.LoadSynchronous();
		return LightProbePPM_32.LoadSynchronous();
	}
};
