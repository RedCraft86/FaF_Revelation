// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldEvents/WorldEventBase.h"
#include "MiscEvents.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Misc", meta = (Hidden))
struct TOROGAMEPLAY_API FWEMiscBase : public FWorldEventBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Remote Event")
struct TOROGAMEPLAY_API FWEEventRemote final : public FWEMiscBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteEvent")
		FName EventName;

	FWEEventRemote() : EventName(NAME_None) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Actor Event")
struct TOROGAMEPLAY_API FWEEventActor final : public FWEMiscBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActorEvent")
		FName EventName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActorEvent")
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWEEventActor() : EventName(NAME_None), Targets({}) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Sequencer")
struct TOROGAMEPLAY_API FWESequencer final : public FWEMiscBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Sequencer")
		bool bStopPlaying;

	UPROPERTY(EditAnywhere, Category = "Sequencer", meta = (EditCondition = "!bStopPlaying", EditConditionHides))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		TSet<TSoftObjectPtr<class ALevelSequenceActor>> Targets;

	FWESequencer() : bStopPlaying(false), PlayRate(1.0f) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Load Stream Level")
struct TOROGAMEPLAY_API FWELevelStreamLoad final : public FWEMiscBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteEvent")
		TSoftObjectPtr<UWorld> World;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteEvent")
		bool bMakeVisibleAfterLoad;

	FWELevelStreamLoad() : World(nullptr), bMakeVisibleAfterLoad(true) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Unload Stream Level")
struct TOROGAMEPLAY_API FWELevelStreamUnload final : public FWEMiscBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RemoteEvent")
		TSoftObjectPtr<UWorld> World;

	FWELevelStreamUnload() : World(nullptr) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};
