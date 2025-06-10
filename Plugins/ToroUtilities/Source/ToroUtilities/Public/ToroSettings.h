// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "Helpers/ClassGetters.h"
#include "MiscObjects/UDSSetter.h"
#include "GeneralProjectSettings.h"
#include "SaveSystem/ToroNativeSaves.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Utilities")
class TOROUTILITIES_API UToroSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings(): WordsPerSecond(2)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");

		SaveObjects = {
			{SaveTags::TAG_GlobalSave, UToroGlobalSave::StaticClass()},
			{SaveTags::TAG_GameSave, UToroGameSave::StaticClass()},
		};

		LightProbePPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_LightProbe.PPM_LightProbe"));
		BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));

		MainSoundMix = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SMC_Master.SMC_Master"));
	}

	SETTING_GETTER(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> LaunchMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UInputMappingContext> InputMappings;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSet<TSoftClassPtr<UUserWidget>> DefaultWidgets;
	
	UPROPERTY(Config, EditAnywhere, Category = SaveSystem)
		FName DemoName;

	UPROPERTY(Config, EditAnywhere, Category = SaveSystem, meta = (ForceInlineRow, Categories = "Save"))
		TMap<FGameplayTag, TSoftClassPtr<UToroSaveObject>> SaveObjects;

	UPROPERTY(Config, EditAnywhere, Category = Rendering)
		TSoftClassPtr<UUDSSetterObject> UDS_Setter;

	UPROPERTY(Config, EditAnywhere, Category = Rendering)
		TSoftObjectPtr<UMaterialInterface> LightProbePPM;

	UPROPERTY(Config, EditAnywhere, Category = Rendering)
		TSoftObjectPtr<UMaterialInterface> BrightnessPPM;

	UPROPERTY(Config, EditAnywhere, Category = Audio)
		TSoftObjectPtr<USoundMix> MainSoundMix;

	float GetReadingTime(const FString& InString) const
	{
		TArray<FString> Words;
		InString.ParseIntoArray(Words, TEXT(" "));
		return Words.Num() / FMath::Max(1.0f, (float)WordsPerSecond);
	}

	bool IsOnLaunchMap(const UObject* WorldContext) const
	{
		if (LaunchMap.IsNull()) return false;
		return UGameplayStatics::GetCurrentLevelName(WorldContext) == LaunchMap.GetAssetName();
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
};