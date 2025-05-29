// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "Helpers/GameplayTagHelpers.h"
#include "WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Misc Actions", meta = (Hidden))
struct TOROUTILITIES_API FWTaskMiscActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Level Streaming")
struct TOROUTILITIES_API FWTaskLevelStreaming final : public FWTaskMiscActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelStreaming)
		bool bUnloadLevel;
	
	UPROPERTY(EditAnywhere, Category = LevelStreaming, meta = (EditCondition = "!bUnloadLevel", EditConditionHides))
		bool bMakeVisibleAfterLoad;
	
	UPROPERTY(EditAnywhere, Category = LevelStreaming)
		TSoftObjectPtr<UWorld> Level;

	FWTaskLevelStreaming(): bUnloadLevel(false), bMakeVisibleAfterLoad(true) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Level Sequence")
struct TOROUTILITIES_API FWTaskLevelSequence final : public FWTaskMiscActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelSequence)
		bool bStopPlaying;

	UPROPERTY(EditAnywhere, Category = LevelSequence, meta = (EditCondition = "!bStopPlaying", EditConditionHides))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = LevelSequence)
		TSet<TSoftObjectPtr<class AToroSequenceActor>> Targets;

	FWTaskLevelSequence(): bStopPlaying(false), PlayRate(1.0f) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Grant Achievement")
struct TOROUTILITIES_API FWTaskAchievement final : public FWTaskMiscActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Achievement, meta = (Categories = "Achievement"))
		FGameplayTag AchievementID;

	UPROPERTY(EditAnywhere, Category = Achievement, meta = (ClampMin = 1, UIMin = 1, ClampMax = 200, UIMax = 200))
		uint8 Progress;

	FWTaskAchievement(): AchievementID(GAMEPLAY_TAG_BASE(Achievement)), Progress(1) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sound Actions", meta = (Hidden))
struct TOROUTILITIES_API FWTaskSoundActions : public FWTaskMiscActions
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "World Sound")
struct TOROUTILITIES_API FWTaskWorldSound final : public FWTaskSoundActions
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
		TSet<TSoftObjectPtr<AAmbientSound>> Targets;

	FWTaskWorldSound()
		: bStopSound(false), StartTime(0.0f), bFade(false)
		, FadeTime(1.0f), FadeCurve(EAudioFaderCurve::Linear)
	{}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "2D Sound")
struct TOROUTILITIES_API FWTaskSound2D final : public FWTaskSoundActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Sound2D)
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Sound2D, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;
	
	UPROPERTY(EditAnywhere, Category = Sound2D, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	FWTaskSound2D(): Volume(1.0f), Pitch(1.0f) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};