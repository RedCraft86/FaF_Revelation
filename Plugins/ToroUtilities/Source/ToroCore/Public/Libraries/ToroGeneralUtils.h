// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroGeneralUtils.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FActorBoundsCheckParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		TEnumAsByte<ECollisionChannel> LineTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		FVector BoundingBoxLerp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bOnlyCollidingComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bIncludeFromChildActors;

	FActorBoundsCheckParams()
		: LineTraceChannel(ECC_Visibility)
		, BoundingBoxLerp(FVector(1.0f))
		, bOnlyCollidingComponents(true)
		, bIncludeFromChildActors(false)
	{}
	
	FActorBoundsCheckParams(const ECollisionChannel InLineTraceChannel, const FVector& InBoundingBoxLerp,
		const bool bInOnlyCollidingComponents, const bool bInIncludeFromChildActors)
		: LineTraceChannel(InLineTraceChannel)
		, BoundingBoxLerp(InBoundingBoxLerp)
		, bOnlyCollidingComponents(bInOnlyCollidingComponents)
		, bIncludeFromChildActors(bInIncludeFromChildActors)
	{}
	
	FActorBoundsCheckParams(const float InBoundingBoxLerp)
		: LineTraceChannel(ECC_Visibility)
		, BoundingBoxLerp(FVector(InBoundingBoxLerp))
		, bOnlyCollidingComponents(true)
		, bIncludeFromChildActors(false)
	{}
	
	TArray<FVector> ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const;
};

UCLASS(DisplayName = "Toro Utilities")
class TOROCORE_API UToroGeneralUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = Windows)
		static EWindowsDialogueResult OpenWindowsDialogue(const FString Title, const FString Message,
			const EWindowsDialogueType Type, const EWindowsDialogueIcon Icon);

	UFUNCTION(BlueprintCallable, Category = Windows)
		static void TriggerVirtualKey(const FKey InKey, const EVirtualKeyTriggerType Type, const bool bRepeat);

	/* Gets the vertices/corner locations of the actor's bounding box.
	* @param Target - Actor to use.
	* @param bOnlyCollidingComponents - If enabled, only components that can collide will be used in calculation.
	* @param bIncludeFromChildActors - Whether child actors should be included in the calculation.
	* @param Origin - Origin of the actor.
	* @param BoxExtent - Bounding box extent of the actor.
	* @return Locations of the 8 (or less) vertices of the bounding box.
	*/
	UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target"))
		static TArray<FVector> GetBoundingBoxVertices(const AActor* Target, const bool bOnlyCollidingComponents,
			const bool bIncludeFromChildActors, FVector& Origin, FVector& BoxExtent);
	
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
		const bool bLineTrace = true, const FActorBoundsCheckParams& TraceParams = FActorBoundsCheckParams());

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

	/* Calls a remote event by name anywhere. */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = "ContextObject"))
		static void CallRemoteEvent(UObject* ContextObject, const FName EventName);

	/* Force a garbage collection to take place to hopefully reduce memory usage */
	UFUNCTION(BlueprintCallable, Category = Loading)
		static void ForceGarbageCollection();

	/* Get the number of currently loading async packages */
	UFUNCTION(BlueprintCallable, Category = Loading, DisplayName = "Get Num Async Packages")
		static int32 GetNumAsyncPkgs();

	/* Get the loading progress of an async package, otherwise -1 */
	UFUNCTION(BlueprintCallable, Category = Loading, DisplayName = "Get Async Load Percentage")
		static float GetAsyncPkgLoadPercent(const FString Package);

	/* Calls an event by name in the Target object. */
	UFUNCTION(BlueprintCallable, Category = Object, meta = (DefaultToSelf = "Target"))
		static void CallLocalEvent(UObject* Target, const FName EventName);

	/* EDITOR ONLY - Adds a component to an actor similar to the add component functions but also refreshes component list to include this new one. */
	UFUNCTION(BlueprintCallable, Category = Editor, meta = (DeterminesOutputType = "InClass", DynamicOutputParam = "ReturnValue"))
		static UActorComponent* AddActorInstanceComponent(AActor* Target, const TSubclassOf<UActorComponent> InClass);
};
