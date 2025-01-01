// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Misc Actions", meta = (Hidden))
struct TORORUNTIME_API FWTaskMiscActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Level Streaming")
struct TORORUNTIME_API FWTaskLevelStreaming final : public FWTaskMiscActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelStreaming)
		bool bUnloadLevel;
	
	UPROPERTY(EditAnywhere, Category = LevelStreaming, meta = (EditCondition = "!bUnloadLevel", EditConditionHides))
		bool bMakeVisibleAfterLoad;
	
	UPROPERTY(EditAnywhere, Category = LevelStreaming)
		TSoftObjectPtr<UWorld> Level;

	FWTaskLevelStreaming() : bUnloadLevel(false), bMakeVisibleAfterLoad(true) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Level Sequence")
struct TORORUNTIME_API FWTaskLevelSequence final : public FWTaskMiscActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelSequence)
		bool bStopPlaying;

	UPROPERTY(EditAnywhere, Category = LevelSequence, meta = (EditCondition = "!bStopPlaying", EditConditionHides))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = LevelSequence)
		TSet<TSoftObjectPtr<class ALevelSequenceActor>> Targets;

	FWTaskLevelSequence() : bStopPlaying(false), PlayRate(1.0f) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};