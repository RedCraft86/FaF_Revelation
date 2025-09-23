#include "ToroRuntime.h"

DEFINE_LOG_CATEGORY(LogToroRuntime);

#define LOCTEXT_NAMESPACE "ToroRuntime"

UToroSettings::UToroSettings()
	: MapRegistry({
		  {EToroMapType::MainMenu, nullptr},
		  {EToroMapType::Gameplay, nullptr},
		  {EToroMapType::Extras, nullptr}
	}), MaxProbes(16)
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroRuntime");

	LightProbePPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_LightProbe.PPM_LightProbe"));
	BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroRuntimeModule, ToroRuntime)