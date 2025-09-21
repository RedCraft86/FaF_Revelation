// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroSettings.h"

UToroSettings::UToroSettings()
	: MapRegistry({
		{EToroMapType::MainMenu, nullptr},
		{EToroMapType::Gameplay, nullptr},
		{EToroMapType::Extras, nullptr}
	})
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroRuntime");
}

bool UToroSettings::IsOnMap(const UObject* ContextObject, const EToroMapType MapType)
{
	const TSoftObjectPtr<UWorld>* World = MapRegistry.Find(MapType);
	return World && UGameplayStatics::GetCurrentLevelName(ContextObject) == World->GetAssetName();
}

#if WITH_EDITOR
void UToroSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UToroSettings, MapRegistry))
	{
		MapRegistry.FindOrAdd(EToroMapType::MainMenu);
		MapRegistry.FindOrAdd(EToroMapType::Gameplay);
		MapRegistry.FindOrAdd(EToroMapType::Extras);
	}
}
#endif