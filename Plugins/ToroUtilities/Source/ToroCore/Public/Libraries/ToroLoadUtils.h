// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroLoadUtils.generated.h"

UCLASS(DisplayName = "Load Utils")
class TOROCORE_API UToroLoadUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = LoadUtils)
		static void ForceGarbageCollection();

	UFUNCTION(BlueprintCallable, Category = LoadUtils)
		static bool IsAsyncPackageLoading(const FString Package);
	
	UFUNCTION(BlueprintCallable, Category = LoadUtils)
		static float GetPackageAsyncLoadPercent(const FString Package);

	UFUNCTION(BlueprintCallable, Category = LoadUtils, meta = (WorldContext = "ContextObject"))
		static void FlushLevelStreaming(const UObject* ContextObject);
	
	UFUNCTION(BlueprintCallable, Category = LoadUtils, meta = (WorldContext = "ContextObject"))
		static void ApplyDefaultPriorityLoading(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = LoadUtils, meta = (WorldContext = "ContextObject"))
		static void ApplyStreamingPriorityLoading(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = LoadUtils, meta = (WorldContext = "ContextObject"))
		static void ApplyHighestPriorityLoading(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = LoadUtils, meta = (WorldContext = "ContextObject"))
		static void ApplyCustomPriorityLoading(const UObject* ContextObject, bool UseHighPriorityLoading, float MaxAsyncLoadingMilliSeconds, float MaxActorUpdateMilliSeconds);

private:

	static float DefaultLevelStreamingActorsUpdateTimeLimit;
	static float DefaultLevelStreamingComponentsRegistrationGranularity;
	static float DefaultAsyncLoadingTimeLimit;

	static bool HasCapturedDefaults;
	static void CaptureDefaultLoadingSettings();
};
