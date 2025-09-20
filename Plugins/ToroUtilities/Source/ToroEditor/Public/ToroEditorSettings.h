// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroEditorSettings.generated.h"

UCLASS(Config = Editor, DefaultConfig, DisplayName = "Toro Editor")
class TOROEDITOR_API UToroEditorSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroEditorSettings();
	
	SETTING_GETTER(UToroEditorSettings)

	UPROPERTY(Config, EditAnywhere, Category = Editor)
		TMap<FString, FString> StartupCommands;
	
	UPROPERTY(Config, EditAnywhere, Category = Editor)
		FDirectoryPath AssetLibrary;

	FString GetAssetLibraryPath() const;

private:

	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
