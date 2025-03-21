// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UDSInterface.h"
#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "InputMappingContext.h"
#include "GeneralProjectSettings.h"
#include "SaveSystem/ToroSaveTypes.h"
#include "DataTypes/LocalSoundTypes.h"
#include "Achievements/AchievementTypes.h"
#include "UserSettings/UserSettingTypes.h"
#include "GuidePages/GuidePageTypes.h"
#include "Engine/DeveloperSettings.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Utilities")
class TOROUTILITIES_API UToroSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings() : WordsPerSecond(2)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");

		SaveObjects = {
			{Tag_Save_Global, UToroGlobalSave::StaticClass()},
			{Tag_Save_Game, UToroGameSave::StaticClass()},
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

	SETTING_GETTER_MUTABLE(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;

	UPROPERTY(Config, EditAnywhere, Category = Runtime, DisplayName = "UDS Actor")
		TSoftClassPtr<AActor> UDSActor;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UAchievementDatabase> AchievementDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UGuidePageDatabase> GuidePageDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<ULocalSoundDatabase> LocalSoundDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UInputMappingContext> DefaultInputMappings;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSet<TSoftClassPtr<UUserWidget>> DefaultWidgets;

	UPROPERTY(Config, EditAnywhere, Category = SaveSystem)
		FName DemoName;

	UPROPERTY(Config, EditAnywhere, Category = SaveSystem, meta = (ForceInlineRow, Categories = "Save"))
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

	void SetUDSSettings(const UObject* WorldContext, const FUDSSettings& Settings) const
	{
		IUDSInterface::SetSettings(UGameplayStatics::GetActorOfClass(
			WorldContext, UDSActor.LoadSynchronous()), Settings);
	}

	bool IsOnGameplayMap(const UObject* WorldContext) const
	{
		if (GameplayMap.IsNull()) return false;
		return UGameplayStatics::GetCurrentLevelName(WorldContext) == GameplayMap.GetAssetName();
	}

	FString GetDemoName() const
	{
		return DemoName.IsNone() ? TEXT("") : DemoName.ToString().Replace(TEXT(" "), TEXT(""));
	}

	FText GetVersionLabel() const
	{
		FText Result;
		if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
		{
			Result = FText::Format(INVTEXT("Version: {0}"), FText::FromString(ProjectSettings->ProjectVersion));
		}
		if (!DemoName.IsNone())
		{
			Result = FText::Format(INVTEXT("{0} | {1}"), Result, FText::FromName(DemoName));
		}
		return FText::Format(INVTEXT("{0} | {1} Build"), Result,
			FText::FromString(LexToString(FApp::GetBuildConfiguration())));
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
