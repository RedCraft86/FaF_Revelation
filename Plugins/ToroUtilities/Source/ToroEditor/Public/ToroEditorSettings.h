// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterHelpers.h"
#include "ToroEditorSettings.generated.h"

UCLASS(Config = Editor, DefaultConfig, DisplayName = "Editor Settings")
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
		CategoryName = TEXT("ToroUtilities");
		SectionName = TEXT("EditorSettings");
	}

	SETTING_GETTER(UToroEditorSettings)

	UPROPERTY(Config, EditAnywhere, Category = "Editor")
		TMap<FString, FString> StartupCommands;
};