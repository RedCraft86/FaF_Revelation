// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroRuntimeSettings.h"

UToroRuntimeSettings::UToroRuntimeSettings()
	: MapRegistry({
		{EToroMapType::MainMenu, nullptr},
		{EToroMapType::Gameplay, nullptr},
		{EToroMapType::Extras, nullptr}
	})
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroRuntime");
}

bool UToroRuntimeSettings::IsOnMapType(const UObject* ContextObject, const EToroMapType MapType)
{
	const TSoftObjectPtr<UWorld>* World = MapRegistry.Find(MapType);
	return World && UGameplayStatics::GetCurrentLevelName(ContextObject) == World->GetAssetName();
}

#if WITH_EDITOR
void UToroRuntimeSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UToroRuntimeSettings, MapRegistry))
	{
		MapRegistry.FindOrAdd(EToroMapType::MainMenu);
		MapRegistry.FindOrAdd(EToroMapType::Gameplay);
		MapRegistry.FindOrAdd(EToroMapType::Extras);
	}
}
#endif