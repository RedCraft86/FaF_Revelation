// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroAsyncActionBase.h"
#include "Blueprint/UserWidget.h"
#include "AsyncPlayWidgetAnim.generated.h"

UCLASS()
class TOROCORE_API UAsyncPlayWidgetAnim final : public UToroAsyncActionBase
{
	GENERATED_BODY()
	
public:

	/* Plays a widget animation.
	* @param Target - User Widget to play the animation on.
	* @param InAnimation - Animation to play.
	* @param PlaybackSpeed - The speed multiplier to play the animation with. Minimum: 0.05f
	* @param PlayDirection - Direction to play the animation to.
	*/
	UFUNCTION(BlueprintCallable, Category = "UserInterface|Animation", meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Target", DisplayName = "Play Widget Animation"))
		static UAsyncPlayWidgetAnim* PlayWidgetAnimation(UUserWidget* Target, UWidgetAnimation* InAnimation, const float InPlaybackSpeed = 1.0f,
			const EUMGSequencePlayMode::Type InPlayDirection = EUMGSequencePlayMode::Forward);

	UPROPERTY(BlueprintAssignable)
		FToroAsyncActionDelegate OnFinished;
	
private:

	UPROPERTY(Transient) TObjectPtr<UUserWidget> Widget = nullptr;
	UPROPERTY(Transient) TObjectPtr<UWidgetAnimation> Animation = nullptr;
	EUMGSequencePlayMode::Type PlayDirection = EUMGSequencePlayMode::Forward;
	float PlaybackSpeed = 1.0f;
	
	virtual void Activate() override;
	void OnFinishPlaying(UUMGSequencePlayer& InPlayer);
};
