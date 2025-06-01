// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable UnrealHeaderToolError
#pragma once

#include "Components/AudioComponent.h"
#include "WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Misc", meta = (Hidden))
struct TOROUTILITIES_API FWATaskMisc : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Level Streaming")
struct TOROUTILITIES_API FWALevelStreaming final : public FWATaskMisc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelStreaming)
		bool bUnloadLevel;
	
	UPROPERTY(EditAnywhere, Category = LevelStreaming, meta = (EditCondition = "!bUnloadLevel", EditConditionHides))
		bool bMakeVisibleAfterLoad;
	
	UPROPERTY(EditAnywhere, Category = LevelStreaming)
		TSoftObjectPtr<UWorld> Level;

	FWALevelStreaming(): bUnloadLevel(false), bMakeVisibleAfterLoad(true) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Level Sequence")
struct TOROUTILITIES_API FWALevelSequence final : public FWATaskMisc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelSequence)
		bool bStopPlaying;

	UPROPERTY(EditAnywhere, Category = LevelSequence, meta = (EditCondition = "!bStopPlaying", EditConditionHides))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = LevelSequence)
		TSet<TSoftObjectPtr<class AToroSequenceActor>> Targets;

	FWALevelSequence(): bStopPlaying(false), PlayRate(1.0f) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sound", meta = (Hidden))
struct TOROUTILITIES_API FWATaskSounds : public FWATaskMisc
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "World Sound")
struct TOROUTILITIES_API FWAWorldSound final : public FWATaskSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = WorldSound)
		bool bStopSound;
	
	UPROPERTY(EditAnywhere, Category = WorldSound, meta = (ClampMin = 0.0f, UIMin = 0.0f, EditCondition = "!bStopSound", EditConditionHides))
		float StartTime;
	
	UPROPERTY(EditAnywhere, Category = WorldSound)
		bool bFade;

	UPROPERTY(EditAnywhere, Category = WorldSound, meta = (ClampMin = 0.1f, UIMin = 0.1f, EditCondition = "bFade", EditConditionHides))
		float FadeTime;
	
	UPROPERTY(EditAnywhere, Category = WorldSound, meta = (EditCondition = "bFade", EditConditionHides))
		EAudioFaderCurve FadeCurve;
	
	UPROPERTY(EditAnywhere, Category = WorldSound)
		TSet<TSoftObjectPtr<class AAmbientSound>> Targets;

	FWAWorldSound()
		: bStopSound(false), StartTime(0.0f), bFade(false)
		, FadeTime(1.0f), FadeCurve(EAudioFaderCurve::Linear)
	{}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "2D Sound")
struct TOROUTILITIES_API FWASound2D final : public FWATaskSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Sound2D, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;
	
	UPROPERTY(EditAnywhere, Category = Sound2D, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	UPROPERTY(EditAnywhere, Category = Sound2D)
		TObjectPtr<USoundBase> Sound;

	FWASound2D(): Volume(1.0f), Pitch(1.0f) {}

protected:

	virtual void RunAction() override;
};