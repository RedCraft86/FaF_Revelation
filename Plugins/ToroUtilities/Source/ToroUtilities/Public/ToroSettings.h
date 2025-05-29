// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputMappingContext.h"
#include "GameplayTagContainer.h"
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

	UToroSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroUtilities");

		SaveObjects = {
			{Tag_Save_Global, UToroGlobalSave::StaticClass()},
			{Tag_Save_Game, UToroGameSave::StaticClass()},
		};
	}

	SETTING_GETTER_MUTABLE(UToroSettings)

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

	FString GetDemoName() const
	{
		return DemoName.IsNone() ? TEXT("") : DemoName.ToString().Replace(TEXT(" "), TEXT(""));
	}
};
