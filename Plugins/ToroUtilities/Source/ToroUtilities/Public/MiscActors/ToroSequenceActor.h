// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LevelSequenceActor.h"
#include "DataTypes/InputModeData.h"
#include "MovieSceneSequencePlayer.h"
#include "ToroSequenceActor.generated.h"

UCLASS()
class TOROUTILITIES_API AToroSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()

public:

	AToroSequenceActor(const FObjectInitializer& Init): Super(Init), bLockPlayer(false), Skippable(false) {}

	UPROPERTY(EditAnywhere, Category = General)
		bool bLockPlayer;

	UPROPERTY(EditAnywhere, Category = General)
		bool Skippable;

	UFUNCTION(BlueprintPure, Category = Actor)
		FGuid GetCutsceneGuid() const { return CutsceneGuid; }

	UFUNCTION(BlueprintCallable, Category = "Sequencer|Player")
		void Play();

	UFUNCTION(BlueprintCallable, Category = "Sequencer|Player")
		void Reverse();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void Stop();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SkipToEnd();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void StopAtCurrentTime();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SetPlayRate(const float InRate) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SetPlaybackPosition(FMovieSceneSequencePlaybackParams InParams) const;

	DECLARE_MULTICAST_DELEGATE(FOnFinishedEvent)
	FOnFinishedEvent OnFinishedEvent;
	
private:

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid CutsceneGuid;

	UPROPERTY() FGameInputModeData CachedInputMode;
	UPROPERTY(Transient) TObjectPtr<class UCutsceneSkipWidget> Widget;

	void OnFinished();
	void LockPlayer();
	void UnlockPlayer();

	UCutsceneSkipWidget* GetWidget();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
