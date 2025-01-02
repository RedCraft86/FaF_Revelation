// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ClassGetterHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "ToroEditorSettings.generated.h"

UCLASS(Config = Editor, DefaultConfig, DisplayName = "Toro - Editor")
class TOROEDITOR_API UToroEditorSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UToroEditorSettings()
		: StartupCommands({
			{ TEXT("r.VSyncEditor"), TEXT("1") },
			{ TEXT("r.Streaming.PoolSize"), TEXT("3000") }
		})
	{
		CategoryName = TEXT("Project");
		SectionName = TEXT("ToroEditor");
	}

	SETTING_GETTER(UToroEditorSettings)

	UPROPERTY(Config, EditAnywhere, Category = "Editor")
		TMap<FString, FString> StartupCommands;
};