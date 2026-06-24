// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameSettings.h"
#include "Kismet/GameplayStatics.h"

UGameSettings::UGameSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("GameSettings");
}

bool UGameSettings::IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const
{
	const TSoftObjectPtr<UWorld>& ExpectedMap = GameMaps[static_cast<uint8>(MapType)];
	return !ExpectedMap.IsNull() && UGameplayStatics::GetCurrentLevelName(ContextObject) == ExpectedMap.GetAssetName();
}
