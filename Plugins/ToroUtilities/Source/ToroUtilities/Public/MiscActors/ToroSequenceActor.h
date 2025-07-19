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

	AToroSequenceActor(const FObjectInitializer& Init): Super(Init), bLockPlayer(false), bSkippable(false) {}

	UPROPERTY(EditAnywhere, Category = General)
		bool bLockPlayer;

	UPROPERTY(EditAnywhere, Category = General)
		bool bSkippable;

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
	
protected:

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid CutsceneGuid;

	UPROPERTY() FGameInputModeData CachedInput;
	UPROPERTY(Transient) TObjectPtr<class UCutsceneSkipWidget> Widget;

	virtual void OnFinished();
	virtual void LockPlayer();
	virtual void UnlockPlayer();

	UCutsceneSkipWidget* GetWidget();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
