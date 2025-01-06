// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MetasoundSource.h"
#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "Styles/ExpressiveTextStyleBase.h"
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
	}

	SETTING_GETTER(UToroRuntimeSettings)
	
	UPROPERTY(Config, EditAnywhere, Category = "Runtime")
		TSoftObjectPtr<UMetaSoundSource> DefaultTheme;
	
	UPROPERTY(Config, EditAnywhere, Category = "Runtime")
		TSoftObjectPtr<UExpressiveTextStyleBase> DefaultTextStyle;
};
