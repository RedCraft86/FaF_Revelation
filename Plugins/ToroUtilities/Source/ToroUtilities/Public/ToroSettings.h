// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "SaveSystem/ToroSaveTypes.h"
#include "DataTypes/LocalSoundTypes.h"
#include "UserSettings/UserSettingTypes.h"
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
	}

	SETTING_GETTER_MUTABLE(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UAchievementDatabase> AchievementDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<ULocalSoundDatabase> LocalSoundDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

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

	bool IsOnGameplayMap(const UObject* WorldContext) const
	{
		if (GameplayMap.IsNull()) return false;
		return UGameplayStatics::GetCurrentLevelName(WorldContext) == GameplayMap.GetAssetName();
	}

	FString GetDemoName() const
	{
		const FString Str = DemoName.IsNone() ? TEXT("") : DemoName.ToString().Replace(TEXT(" "), TEXT(""));
		return Str.IsEmpty() || Str == TEXT(" ") || Str.TrimStartAndEnd().IsEmpty() ? TEXT("") : Str;
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
