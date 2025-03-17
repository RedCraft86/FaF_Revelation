// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "DataTypes/LocalSoundTypes.h"
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
	}

	SETTING_GETTER_MUTABLE(UToroSettings)

	UPROPERTY(Config, EditAnywhere, Category = Runtime, meta = (ClampMin = 1, UIMin = 1))
		uint8 WordsPerSecond;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UWorld> GameplayMap;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<ULocalSoundDatabase> LocalSoundDatabase;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;

	UPROPERTY(Config, EditAnywhere, Category = Runtime)
		TSet<TSoftClassPtr<UUserWidget>> DefaultWidgets;

	UPROPERTY(Config, EditAnywhere, Category = SaveSystem)
		FName DemoVersion;

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
};
