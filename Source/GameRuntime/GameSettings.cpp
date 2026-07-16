// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameSettings.h"
#include "Kismet/GameplayStatics.h"

UGameSettings::UGameSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("GameSettings");
}

EGameMapType UGameSettings::GetCurrentMapType(const UObject* ContextObject) const
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(ContextObject);
	for (int32 i = 0; i < static_cast<int32>(EGameMapType::MAX); i++)
	{
		if (LevelName.Equals(GameMaps[i].GetAssetName()))
		{
			return static_cast<EGameMapType>(i);
		}
	}

	return EGameMapType::Gameplay;
}

bool UGameSettings::IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const
{
	const TSoftObjectPtr<UWorld>& ExpectedMap = GameMaps[static_cast<uint8>(MapType)];
	return !ExpectedMap.IsNull() && UGameplayStatics::GetCurrentLevelName(ContextObject) == ExpectedMap.GetAssetName();
}
