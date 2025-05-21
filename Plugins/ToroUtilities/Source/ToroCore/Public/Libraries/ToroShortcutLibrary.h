// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroShortcutLibrary.generated.h"

UCLASS(DisplayName = "Shortcuts Library")
class TOROCORE_API UToroShortcutLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/* Stops camera fading and clears the currently faded state. */
	UFUNCTION(BlueprintCallable, Category = CameraFades, meta = (WorldContext = "ContextObject", Keywords = "Fade Camera"))
	static void ClearCameraFade(const UObject* ContextObject)
	{
		if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
		{
			PCM->StopCameraFade();
		}
	}

	/* Turns on camera fading at the given opacity. Does not auto-animate, allowing user to animate themselves.
	* @param Alpha - Alpha/Amount to set the fade to.
	* @param Color - Color of the fade.
	* @param bFadeAudio - True to fade audio volume along with the alpha of the solid color.
	*/
	UFUNCTION(BlueprintCallable, Category = CameraFades, meta = (WorldContext = "ContextObject", Keywords = "Fade Camera"))
	static void SetCameraFade(const UObject* ContextObject, const float Alpha, const FLinearColor Color, const bool bFadeAudio)
	{
		if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
		{
			PCM->SetManualCameraFade(Alpha, Color, bFadeAudio);
		}
	}

	/* Does a camera fade to/from a solid color. Animates automatically.
	* @param FromAlpha - Alpha at which to begin the fade. Range [0..1], where 0 is fully transparent and 1 is fully opaque solid color.
	* @param ToAlpha - Alpha at which to finish the fade.
	* @param Duration - How long the fade should take, in seconds.
	* @param Color - Color to fade to/from.
	* @param bShouldFadeAudio - True to fade audio volume along with the alpha of the solid color.
	* @param bHoldWhenFinished - True for fade to hold at the ToAlpha until explicitly stopped (e.g. with StopCameraFade).
	*/
	UFUNCTION(BlueprintCallable, Category = CameraFades, meta = (WorldContext = "ContextObject", Keywords = "Fade Camera", AdvancedDisplay = "Color, bHoldWhenFinished"))
	static void StartCameraFade(UObject* ContextObject, const float FromAlpha, const float ToAlpha = 1.0f, const float Duration = 1.0f,
		const FLinearColor Color = FLinearColor::Black, const bool bShouldFadeAudio = true, const bool bHoldWhenFinished = true)
	{
		if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, 0))
		{
			PCM->StartCameraFade(FromAlpha, ToAlpha, Duration, Color, bShouldFadeAudio, bHoldWhenFinished);
		}
	}

	/* Set the view target blending with variable control.
	* @param NewTarget - New actor to set as view target.
	* @param BlendTime - Time taken to blend.
	* @param BlendFunc - Cubic, Linear etc functions for blending.
	* @param BlendExp -  Exponent, used by certain blend functions to control the shape of the curve.
	* @param bLockOutgoing - If true, lock outgoing view target to last frame's camera position for the remainder of the blend.
	*/
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (WorldContext = "ContextObject", DefaultToSelf = "NewTarget", Keywords = "Camera", AdvancedDisplay = "BlendFunc, BlendExp, bLockOutgoing"))
	static void SetViewTarget(UObject* ContextObject, AActor* NewTarget, const float BlendTime = 1.0f,
		const EViewTargetBlendFunction BlendFunc = VTBlend_Linear, const float BlendExp = 0.0f, const bool bLockOutgoing = false)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(ContextObject, 0))
		{
			PC->SetViewTargetWithBlend(NewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
		}
	}

	/* Adds an actor tag. */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Add Tag", meta = (DefaultToSelf = "Target"))
	static void AddActorTag(AActor* Target, const FName InTag) { if (IsValid(Target)) Target->Tags.AddUnique(InTag); }

	/* Removes an actor tag. */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Remove Tag", meta = (DefaultToSelf = "Target"))
	static void RemoveActorTag(AActor* Target, const FName InTag) { if (IsValid(Target)) Target->Tags.Remove(InTag); }

	/* EDITOR ONLY - Adds a component to an actor similar to the add component functions but also refreshes component list to include this new one. */
	UFUNCTION(BlueprintCallable, Category = Actor, meta = (DeterminesOutputType = "InClass", DynamicOutputParam = "ReturnValue"))
	static UActorComponent* AddActorInstanceComponent(AActor* Target, const TSubclassOf<UActorComponent> InClass)
	{
#if WITH_EDITOR
		if (Target && InClass) 
		{
			UActorComponent* NewComponent = NewObject<UActorComponent>(Target, InClass);
			NewComponent->RegisterComponent();
			Target->AddInstanceComponent(NewComponent);
			return NewComponent;
		}
#endif
		return nullptr;
	}
};