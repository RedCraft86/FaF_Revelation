// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "GameplayTagContainer.h"
#include "GeneralProjectSettings.h"
#include "Helpers/ClassGetters.h"
#include "SaveSystem/SaveGameTypes.h"
#include "SaveSystem/ToroSaveObjects.h"
#include "Achievement/AchievementTypes.h"
#include "Framework/ToroPlayerController.h"
#include "Engine/DeveloperSettings.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Utilities")
class TOROUTILITIES_API UToroSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings(): WordsPerSecond(2), bUseLightProbes(true)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");

		SaveObjects = {
			{Tag_Save_Global, UToroGlobalSave::StaticClass()},
			{Tag_Save_Game, UToroGameSave::StaticClass()},
		};

		LightProbePPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_LightProbes.PPM_LightProbes"));
		BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));

		MainSoundMix = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SMC_Master.SMC_Master"));
		// SoundClasses[0] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Master.SC_Master"));
		// SoundClasses[1] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Music.SC_Music"));
		// SoundClasses[2] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_SoundFX.SC_SoundFX"));
		// SoundClasses[3] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Voice.SC_Voice"));
	}

	SETTING_GETTER_MUTABLE(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UAchievementDatabase> AchievementDatabase;

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

	UPROPERTY(Config, EditAnywhere, Category = LightProbes)
		TSoftObjectPtr<UMaterialInterface> LightProbePPM;

	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<UMaterialInterface> BrightnessPPM;

	UPROPERTY(Config, EditAnywhere, Category = UserSettings)
		TSoftObjectPtr<USoundMix> MainSoundMix;

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
		return bUseLightProbes && !LightProbePPM.IsNull();
	}
};
