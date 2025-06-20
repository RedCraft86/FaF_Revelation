﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/WindowsHelpers.h"
#include "DataTypes/PrimitiveData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroGeneralUtils.generated.h"

UCLASS()
class TOROCORE_API UToroGeneralUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = Windows)
		static EWindowsDialogueResult OpenWindowsDialogue(const FString Title, const FString Message,
			const EWindowsDialogueType Type, const EWindowsDialogueIcon Icon);

	UFUNCTION(BlueprintCallable, Category = Windows)
		static void TriggerVirtualKey(const FKey InKey, const EVirtualKeyTriggerType Type, const bool bRepeat);
	
	/* Checks if actor is on the player screen.
	* @param Target - Actor to check.
	* @param MaxDistance - The maximum distance before the actor is considered no longer in sight.
	* @param bOriginOnly - If enabled, the checks will only make use of the actor's origin, bounds will not be used.
	* @param bLineTraceCheck - Draw a line trace from the camera to the target when checking if it's in player view.
	* @param TraceCheckParams - Settings to use when testing the different vertices of the actor's bounding box.
	* @return Whether the actor is visible in screen with the given settings.
	*/
	UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target", AutoCreateRefTerm = "TraceCheckParams"))
	static bool IsActorInScreen(const AActor* Target, const float MaxDistance = 5000.0f, const bool bOriginOnly = false,
		const bool bLineTraceCheck = true, const FActorBoundsCheckParams& TraceCheckParams = FActorBoundsCheckParams());

	/* Checks if a Location is in front of the Target. */
	UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target"))
		static bool IsLocationInFront(const AActor* Target, const FVector& Location);

	/* Checks if an Actor is in front of the Target. */
	UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target"))
		static bool IsActorInFront(const AActor* Target, const AActor* ActorToTest);

	/* Tries to get the active Game World. Context is optional but potentially more reliable with it. */
	UFUNCTION(BlueprintPure, Category = World, meta = (AdvancedDisplay = "Context", DefaultToSelf = "Context"))
		static UWorld* GetPlayWorld(const UObject* Context);

	/* Check if we're currently in editor or in a PIE/Standalone game. */
	UFUNCTION(BlueprintPure, Category = World)
		static bool IsInEditor();
	
	/* Restarts the current Level. Returns the Level Name. */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = "ContextObject", AdvancedDisplay = "Options"))
		static void RestartLevel(const UObject* ContextObject, const FString Options = TEXT(""));

	/* Calls an event by name in the Target object. */
	UFUNCTION(BlueprintCallable, Category = Object, meta = (DefaultToSelf = "Target"))
		static void CallLocalEvent(UObject* Target, const FName EventName);

	UFUNCTION(BlueprintCallable, Category = Object)
		static void ForceGarbageCollection();
};