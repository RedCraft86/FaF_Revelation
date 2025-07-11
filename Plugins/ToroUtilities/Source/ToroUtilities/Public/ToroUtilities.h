// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "Helpers/ClassGetters.h"
#include "MiscObjects/UDSSetter.h"
#include "GeneralProjectSettings.h"
#include "SaveSystem/ToroSaveTypes.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "UserSettings/UserSettingTypes.h"
#include "ToroUtilities.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroUtilities, All, All);

class FToroUtilitiesModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

UCLASS(Config = Game, DefaultConfig, DisplayName = "Toro Utilities")
class TOROUTILITIES_API UToroUtilSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroUtilSettings(): WordsPerSecond(2), DemoName(NAME_None)
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");

		DefaultSaves = {{SaveTags::TAG_Global, UToroGlobalSave::StaticClass()}};

		BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));

		MainSoundMix = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SMC_Master.SMC_Master"));
		SoundClasses[0] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Master.SC_Master"));
		SoundClasses[1] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Music.SC_Music"));
		SoundClasses[2] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_SoundFX.SC_SoundFX"));
		SoundClasses[3] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Voice.SC_Voice"));
	}

	SETTING_GETTER(UToroUtilSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> LaunchMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> ExtrasMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftClassPtr<UUDSSetterObject> UDS_Setter;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UInputMappingContext> InputMappings;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSet<TSoftClassPtr<UUserWidget>> DefaultWidgets;
	
	UPROPERTY(Config, EditAnywhere, Category = SaveSystem)
		FName DemoName;

	UPROPERTY(Config, EditAnywhere, Category = SaveSystem, meta = (ForceInlineRow, Categories = "Save"))
		TMap<FGameplayTag, TSoftClassPtr<UToroSaveObject>> DefaultSaves;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<UMaterialInterface> BrightnessPPM;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<USoundMix> MainSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = Settings, meta = (ArraySizeEnum = "/Script/ToroUtilities.ESoundClassType"))
		TSoftObjectPtr<USoundClass> SoundClasses[static_cast<uint8>(ESoundClassType::MAX)];

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
		if (GameplayMap.IsNull() && ExtrasMap.IsNull()) return false;
		const FString LvlName = UGameplayStatics::GetCurrentLevelName(WorldContext);
		return LvlName == GameplayMap.GetAssetName() || LvlName == ExtrasMap.GetAssetName();
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
