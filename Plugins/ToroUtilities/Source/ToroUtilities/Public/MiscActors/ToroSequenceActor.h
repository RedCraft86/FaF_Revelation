// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"
#include "ToroSequenceActor.generated.h"

UCLASS()
class TOROUTILITIES_API AToroSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()

public:

	AToroSequenceActor(const FObjectInitializer& Init): Super(Init), bLockPlayer(false) {}

	UPROPERTY(EditAnywhere, Category = General)
		bool bLockPlayer;

	UFUNCTION(BlueprintCallable, Category = "Sequencer|Player")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "Sequencer|Player")
		void Reverse();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void Stop() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SkipToEnd() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void StopAtCurrentTime() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SetPlayRate(const float InRate) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SetPlaybackPosition(FMovieSceneSequencePlaybackParams InParams) const;

	DECLARE_MULTICAST_DELEGATE(FOnFinishedEvent)
	FOnFinishedEvent OnFinishedEvent;
	
private:

	void OnFinished();
	void LockPlayer();
	void UnlockPlayer() const;
	virtual void BeginPlay() override;
};
