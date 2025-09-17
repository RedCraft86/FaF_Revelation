// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/AudioComponent.h"
#include "Sound/AmbientSound.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroShortcutLibrary.generated.h"

UCLASS(DisplayName = "Shortcut Library")
class TOROCORE_API UToroShortcutLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/* Stops camera fading and clears the currently faded state. */
	UFUNCTION(BlueprintCallable, Category = CameraFades, meta = (WorldContext = "ContextObject", Keywords = "Fade Camera"))
		static void ClearCameraFade(const UObject* ContextObject);

	/* Turns on camera fading at the given opacity. Does not auto-animate, allowing user to animate themselves.
	* @param Alpha - Alpha/Amount to set the fade to.
	* @param Color - Color of the fade.
	* @param bFadeAudio - True to fade audio volume along with the alpha of the solid color.
	*/
	UFUNCTION(BlueprintCallable, Category = CameraFades, meta = (WorldContext = "ContextObject", Keywords = "Fade Camera"))
		static void SetCameraFade(const UObject* ContextObject, const float Alpha, const FLinearColor Color, const bool bFadeAudio);

	/* Does a camera fade to/from a solid color. Animates automatically.
	* @param FromAlpha - Alpha at which to begin the fade. Range [0..1], where 0 is fully transparent and 1 is fully opaque solid color.
	* @param ToAlpha - Alpha at which to finish the fade.
	* @param Duration - How long the fade should take, in seconds.
	* @param Color - Color to fade to/from.
	* @param bShouldFadeAudio - True to fade audio volume along with the alpha of the solid color.
	* @param bHoldWhenFinished - True for fade to hold at the ToAlpha until explicitly stopped (e.g. with StopCameraFade).
	*/
	UFUNCTION(BlueprintCallable, Category = CameraFades, meta = (WorldContext = "ContextObject", Keywords = "Fade Camera", AdvancedDisplay = "Color, bHoldWhenFinished"))
		static void StartCameraFade(UObject* ContextObject, const float FromAlpha = 0.0f, const float ToAlpha = 1.0f, const float Duration = 1.0f,
			const FLinearColor Color = FLinearColor::Black, const bool bShouldFadeAudio = true, const bool bHoldWhenFinished = true);

	/* Set the view target blending with variable control.
	* @param NewTarget - New actor to set as view target.
	* @param BlendTime - Time taken to blend.
	* @param BlendFunc - Cubic, Linear, etc. functions for blending.
	* @param BlendExp -  Exponent, used by certain blend functions to control the shape of the curve.
	* @param bLockOutgoing - If true, lock outgoing view target to last frame's camera position for the remainder of the blend.
	*/
	UFUNCTION(BlueprintCallable, Category = "Game|Player", meta = (WorldContext = "ContextObject", DefaultToSelf = "NewTarget", Keywords = "Camera", AdvancedDisplay = "BlendFunc, BlendExp, bLockOutgoing"))
		static void SetViewTarget(UObject* ContextObject, AActor* NewTarget, const float BlendTime = 1.0f,
			const EViewTargetBlendFunction BlendFunc = VTBlend_Linear, const float BlendExp = 0.0f, const bool bLockOutgoing = false);

	/* Adds an actor tag. */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Add Tag", meta = (DefaultToSelf = "Target"))
		static void AddActorTag(AActor* Target, const FName InTag);

	/* Removes an actor tag. */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Remove Tag", meta = (DefaultToSelf = "Target"))
		static void RemoveActorTag(AActor* Target, const FName InTag);
	
	/* Restarts the current Level. Returns the Level Name. */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = "ContextObject", AdvancedDisplay = "Options"))
		static void RestartLevel(const UObject* ContextObject, const FString Options = TEXT(""));

	/* Calls a remote event by name anywhere. */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = "ContextObject"))
		static void CallRemoteEvent(UObject* ContextObject, const FName EventName);

	/** Begins playing the targeted Audio Component's sound at the designated Start Time, seeking into a sound.
	 * @param StartTime The offset, in seconds, to begin reading the sound at
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Play", meta = (DefaultToSelf = "Target"))
	// ReSharper disable once UnrealHeaderToolError
		static void PlayAmbientSound(const AAmbientSound* Target, float StartTime = 0.0f);

	/* Stop an audio component's sound, issue any delegates if needed */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Stop", meta = (DefaultToSelf = "Target"))
	// ReSharper disable once UnrealHeaderToolError
		static void StopAmbientSound(const AAmbientSound* Target);

	/* Cues request to stop sound after the provided delay (in seconds), stopping immediately if delay is zero or negative */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Stop Delayed", meta = (DefaultToSelf = "Target"))
	// ReSharper disable once UnrealHeaderToolError
		static void StopAmbientSoundDelayed(const AAmbientSound* Target, float DelayTime = 1.0f);

	/** This function allows designers to trigger an adjustment to the sound’s
	 * playback Volume with options for smoothly applying a curve over time.
	 * @param Duration The length of time in which to interpolate between the initial volume and the new volume.
	 * @param TargetLevel The new volume to set the Audio Component to.
	 * @param FadeCurve The curve used when interpolating between the old and new volume.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Adjust Volume", meta = (DefaultToSelf = "Target"))
	// ReSharper disable once UnrealHeaderToolError
		static void AdjustAmbientSoundVolume(const AAmbientSound* Target, float Duration = 1.0f,
			float TargetLevel = 1.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);
	
	/** This function calls Play on an Audio Component instance while applying a volume curve over time.
	 * @param Duration How long it should take to reach the FadeVolumeLevel
	 * @param TargetLevel The percentage of the AudioComponents's calculated volume to fade to
	 * @param StartTime The playback time in which the sound should start from
	 * @param FadeCurve The curve to use when interpolating between the old and new volume
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade In", meta = (DefaultToSelf = "Target"))
	// ReSharper disable once UnrealHeaderToolError
		static void FadeInAmbientSound(const AAmbientSound* Target, float Duration = 1.0f, float TargetLevel = 1.0f,
			float StartTime = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);

	/** This calls a delayed Stop on an Audio Component instance while applying a
	 * volume curve over time. Parameters allow designers to indicate the duration of the fade and the curve shape.
	 * @param Duration how long it should take to reach the FadeVolumeLevel
	 * @param TargetLevel the percentage of the AudioComponents's calculated volume in which to fade to
	 * @param FadeCurve The curve to use when interpolating between the old and new volume
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade Out", meta = (DefaultToSelf = "Target"))
	// ReSharper disable once UnrealHeaderToolError
		static void FadeOutAmbientSound(const AAmbientSound* Target, float Duration = 1.0f,
			float TargetLevel = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);
};
