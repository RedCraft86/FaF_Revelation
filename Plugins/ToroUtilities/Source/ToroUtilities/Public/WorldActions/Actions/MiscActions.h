// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
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

	FWTaskLevelStreaming() : bUnloadLevel(false), bMakeVisibleAfterLoad(true) {}

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

	FWTaskLevelSequence() : bStopPlaying(false), PlayRate(1.0f) {}

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

	FWTaskAchievement() : AchievementID(FGameplayTag::RequestGameplayTag("Achievement")), Progress(1) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Guide Pages")
struct TOROUTILITIES_API FWTaskGuidePages final : public FWTaskMiscActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Achievement, meta = (Categories = "GuidePage"))
		TArray<FGameplayTag> Pages;

	FWTaskGuidePages() {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};