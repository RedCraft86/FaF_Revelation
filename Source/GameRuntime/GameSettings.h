// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UDSInterface.h"
#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "GameSection/GameSectionData.h"
#include "GameSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "Game Settings")
class GAMERUNTIME_API UGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UGameSettings()
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("GameSettings");
	}

	SETTING_GETTER_MUTABLE(UGameSettings)

	UPROPERTY(Config, EditAnywhere, Category = Settings, DisplayName = "UDS Actor")
		TSoftClassPtr<AActor> UDSActor;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
		TSoftObjectPtr<UGameSectionGraph> SectionGraph;

	void SetUDSSettings(const UObject* WorldContext, const FUDSSettings& Settings) const
	{
		IUDSInterface::SetSettings(UGameplayStatics::GetActorOfClass(
			WorldContext, UDSActor.LoadSynchronous()), Settings);
	}
};
