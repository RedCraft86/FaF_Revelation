﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "JsonObjectWrapper.h"
#include "Helpers/ClassGetters.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroConfigs.generated.h"

UCLASS(BlueprintType, DisplayName = "Config Manager")
class TOROUTILITIES_API UToroConfigManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroConfigManager() {}

	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroConfigManager)

	static inline FString Key_Info = TEXT("info");
	static inline FString Key_FirstLaunch = TEXT("first-launch");
	static inline FString Key_DeveloperMode = TEXT("developer-mode");
	static inline FString ConfigPath = FPaths::ProjectSavedDir() / TEXT("game-config.json");

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = GameConfig, DisplayName = "Set Game Config (Boolean)")
		void SetGameConfigBool(const FString& InKey, const bool InValue) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = GameConfig, DisplayName = "Set Game Config (Float)")
		void SetGameConfigFloat(const FString& InKey, const float InValue) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = GameConfig, DisplayName = "Set Game Config (Integer)")
		void SetGameConfigInt(const FString& InKey, const int32 InValue) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = GameConfig, DisplayName = "Set Game Config (String)")
		void SetGameConfigString(const FString& InKey, const FString& InValue) const;

	UFUNCTION(BlueprintPure, Category = GameConfig, DisplayName = "Get Game Config (Boolean)", meta = (AdvancedDisplay = "InDefault"))
		bool GetGameConfigBool(const FString& InKey, const bool InDefault = false) const;

	UFUNCTION(BlueprintPure, Category = GameConfig, DisplayName = "Get Game Config (Float)", meta = (AdvancedDisplay = "InDefault"))
		float GetGameConfigFloat(const FString& InKey, const float InDefault = 0.0f) const;

	UFUNCTION(BlueprintPure, Category = GameConfig, DisplayName = "Get Game Config (Integer)", meta = (AdvancedDisplay = "InDefault"))
		int32 GetGameConfigInt(const FString& InKey, const int32 InDefault = 0) const;

	UFUNCTION(BlueprintPure, Category = GameConfig, DisplayName = "Get Game Config (String)", meta = (AdvancedDisplay = "InDefault"))
		FString GetGameConfigString(const FString& InKey, const FString& InDefault = TEXT("")) const;

	UFUNCTION(BlueprintPure, Category = "GameConfig|Defaults")
		bool IsFirstLaunch() const;
	
	UFUNCTION(BlueprintPure, Category = "GameConfig|Defaults")
		bool IsDeveloperMode() const;
	
private:

	FJsonObjectWrapper GameConfig;

	void LoadGameConfig();
	void SaveGameConfig() const;
	bool CheckConfigDefaults() const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};