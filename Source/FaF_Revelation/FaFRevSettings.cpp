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

FString UFaFRevSettings::GetVersionLabel() const
{
	FString Result;
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		Result.Appendf(TEXT("Version: %s-%s"), *ProjectSettings->ProjectVersion,
			*FString(LexToString(FApp::GetBuildConfiguration())).ToUpper());
	}
	if (!DemoName.IsNone())
	{
		Result.Appendf(TEXT(" | %s"), *DemoName.ToString());
	}
	return Result;
}

bool UFaFRevSettings::IsInMapType(const UObject* ContextObject, const EGameMapType MapType) const
{
	const TSoftObjectPtr<UWorld>& ExpectedMap = GameMaps[static_cast<uint8>(MapType)];
	return !ExpectedMap.IsNull() && UGameplayStatics::GetCurrentLevelName(ContextObject) == ExpectedMap.GetAssetName();
}

UToroDatabase* UFaFRevSettings::GetDatabase(const TSubclassOf<UToroDatabase> Class) const
{
	if (Class)
	{
		for (const TSoftObjectPtr<UToroDatabase>& Database : Databases)
		{
			UToroDatabase* DB = Database.LoadSynchronous();
			if (DB && DB->IsA(Class))
			{
				return DB;
			}
		}
	}
	return nullptr;
}
