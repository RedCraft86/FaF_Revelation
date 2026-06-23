// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "FaFRevSettings.h"
#include "GeneralProjectSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"

UFaFRevSettings::UFaFRevSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("GameSettings");
}

bool UFaFRevSettings::IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const
{
	const TSoftObjectPtr<UWorld>& ExpectedMap = GameMaps[static_cast<uint8>(MapType)];
	return !ExpectedMap.IsNull() && UGameplayStatics::GetCurrentLevelName(ContextObject) == ExpectedMap.GetAssetName();
}
