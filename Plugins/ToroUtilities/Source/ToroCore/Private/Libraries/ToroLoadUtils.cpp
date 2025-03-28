﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroLoadUtils.h"
#include "Engine/CoreSettings.h"

float UToroLoadUtils::DefaultLevelStreamingComponentsRegistrationGranularity;
float UToroLoadUtils::DefaultLevelStreamingActorsUpdateTimeLimit;
float UToroLoadUtils::DefaultAsyncLoadingTimeLimit;
bool UToroLoadUtils::HasCapturedDefaults = false;

void UToroLoadUtils::ForceGarbageCollection()
{
	if (GEngine)
	{
#if WITH_EDITOR
		GEngine->ForceGarbageCollection(false);
#else
		GEngine->ForceGarbageCollection(true);
#endif
	}
}

bool UToroLoadUtils::IsAsyncPackageLoading(const FString Package)
{
	return GetAsyncLoadPercentage(*Package) >= 0.0f;
}

float UToroLoadUtils::GetPackageAsyncLoadPercent(const FString Package)
{
	return GetAsyncLoadPercentage(*Package);
}

void UToroLoadUtils::FlushLevelStreaming(const UObject* ContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		GEngine->BlockTillLevelStreamingCompleted(World);
	}
}

void UToroLoadUtils::ApplyDefaultPriorityLoading(const UObject* ContextObject)
{
	CaptureDefaultLoadingSettings();
	ApplyCustomPriorityLoading(ContextObject, false,
		DefaultAsyncLoadingTimeLimit, DefaultLevelStreamingActorsUpdateTimeLimit);
}

void UToroLoadUtils::ApplyStreamingPriorityLoading(const UObject* ContextObject)
{
	ApplyCustomPriorityLoading(ContextObject, false, 10.0f, 10.0f);
}

void UToroLoadUtils::ApplyHighestPriorityLoading(const UObject* ContextObject)
{
	ApplyCustomPriorityLoading(ContextObject, true, FLT_MAX, FLT_MAX);
}

void UToroLoadUtils::ApplyCustomPriorityLoading(const UObject* ContextObject, const bool UseHighPriorityLoading,
	const float MaxAsyncLoadingMilliSeconds, const float MaxActorUpdateMilliSeconds)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (AWorldSettings* WorldSettings = World->GetWorldSettings())
		{
			GLevelStreamingComponentsRegistrationGranularity = DefaultLevelStreamingComponentsRegistrationGranularity;
			WorldSettings->bHighPriorityLoadingLocal = UseHighPriorityLoading;
			GLevelStreamingActorsUpdateTimeLimit = MaxActorUpdateMilliSeconds;
			GAsyncLoadingUseFullTimeLimit = UseHighPriorityLoading;
			GAsyncLoadingTimeLimit = MaxAsyncLoadingMilliSeconds;
		}
	}
}

void UToroLoadUtils::CaptureDefaultLoadingSettings()
{
	if (!HasCapturedDefaults)
	{
		DefaultLevelStreamingComponentsRegistrationGranularity = GLevelStreamingComponentsRegistrationGranularity;
		DefaultLevelStreamingActorsUpdateTimeLimit = GLevelStreamingActorsUpdateTimeLimit;
		DefaultAsyncLoadingTimeLimit = GAsyncLoadingTimeLimit;
		HasCapturedDefaults = true;
	}
}
