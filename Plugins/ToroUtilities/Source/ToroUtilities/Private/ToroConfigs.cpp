// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroConfigs.h"

#define CFG_WARNING TEXT("Close the game before making changes otherwise they will be overwritten.")

void UToroConfigManager::SetGameConfigBool(const FString& InKey, const bool InValue) const
{
	if (InKey.IsEmpty()) return;
	GameConfig.JsonObject->SetBoolField(InKey, InValue);
	SaveGameConfig();
}

void UToroConfigManager::SetGameConfigFloat(const FString& InKey, const float InValue) const
{
	if (InKey.IsEmpty()) return;
	GameConfig.JsonObject->SetNumberField(InKey, InValue);
	SaveGameConfig();
}

void UToroConfigManager::SetGameConfigInt(const FString& InKey, const int32 InValue) const
{
	if (InKey.IsEmpty()) return;
	GameConfig.JsonObject->SetNumberField(InKey, InValue);
	SaveGameConfig();
}

void UToroConfigManager::SetGameConfigString(const FString& InKey, const FString& InValue) const
{
	if (InKey.IsEmpty()) return;
	GameConfig.JsonObject->SetStringField(InKey, InValue);
	SaveGameConfig();
}

bool UToroConfigManager::GetGameConfigBool(const FString& InKey, const bool InDefault) const
{
	if (InKey.IsEmpty()) return InDefault;
	
	bool RetVal;
	if (!GameConfig.JsonObject->TryGetBoolField(InKey, RetVal))
	{
		RetVal = InDefault;
	}
	return RetVal;
}

float UToroConfigManager::GetGameConfigFloat(const FString& InKey, const float InDefault) const
{
	if (InKey.IsEmpty()) return InDefault;
	
	float RetVal;
	if (!GameConfig.JsonObject->TryGetNumberField(InKey, RetVal))
	{
		RetVal = InDefault;
	}
	return RetVal;
}

int32 UToroConfigManager::GetGameConfigInt(const FString& InKey, const int32 InDefault) const
{
	if (InKey.IsEmpty()) return InDefault;
	
	int32 RetVal;
	if (!GameConfig.JsonObject->TryGetNumberField(InKey, RetVal))
	{
		RetVal = InDefault;
	}
	return RetVal;
}

FString UToroConfigManager::GetGameConfigString(const FString& InKey, const FString& InDefault) const
{
	if (InKey.IsEmpty()) return InDefault;
	
	FString RetVal;
	if (!GameConfig.JsonObject->TryGetStringField(InKey, RetVal))
	{
		RetVal = InDefault;
	}
	return RetVal;
}

bool UToroConfigManager::IsFirstLaunch() const
{
	return GetGameConfigBool(Key_FirstLaunch, true);
}

bool UToroConfigManager::IsDeveloperMode() const
{
#if UE_BUILD_SHIPPING
	return GetGameConfigBool(Key_DeveloperMode, false);
#else
	return GetGameConfigBool(Key_DeveloperMode, true);
#endif
}

void UToroConfigManager::LoadGameConfig()
{
	FString GameConfigStr;
	FFileHelper::LoadFileToString(GameConfigStr, *ConfigPath);
	GameConfig.JsonObjectFromString(GameConfigStr);
	if (CheckConfigDefaults())
	{
		SaveGameConfig();
	}
}

void UToroConfigManager::SaveGameConfig() const
{
	FString GameConfigStr;
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&GameConfigStr, 0);
	FJsonSerializer::Serialize(GameConfig.JsonObject.ToSharedRef(), JsonWriter, true);
	FFileHelper::SaveStringToFile(GameConfigStr, *ConfigPath);
}

bool UToroConfigManager::CheckConfigDefaults() const
{
	bool bNeedsResave = false;
	if (!GameConfig.JsonObject->HasField(Key_Info))
	{
		GameConfig.JsonObject->SetStringField(Key_Info, CFG_WARNING);
		bNeedsResave = true;
	}
	if (!GameConfig.JsonObject->HasField(Key_FirstLaunch))
	{
		GameConfig.JsonObject->SetBoolField(Key_FirstLaunch, true);
		bNeedsResave = true;
	}
	if (!GameConfig.JsonObject->HasField(Key_DeveloperMode))
	{
#if UE_BUILD_SHIPPING
		GameConfig.JsonObject->SetBoolField(Key_DeveloperMode, false);
#else
		GameConfig.JsonObject->SetBoolField(Key_DeveloperMode, true);
#endif
		bNeedsResave = true;
	}
	return bNeedsResave;
}

void UToroConfigManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadGameConfig();
}

void UToroConfigManager::Deinitialize()
{
	Super::Deinitialize();
	SetGameConfigBool(Key_FirstLaunch, false);
}