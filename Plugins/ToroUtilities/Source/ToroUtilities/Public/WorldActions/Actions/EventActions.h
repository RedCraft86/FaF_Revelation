// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "EventActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Event", meta = (Hidden))
struct TOROUTILITIES_API FWAEventActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Remove Event")
struct TOROUTILITIES_API FWAEventRemote final : public FWAEventActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = RemoteEvent)
		FName EventName;

	FWAEventRemote(): EventName(NAME_None) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Actor Event")
struct TOROUTILITIES_API FWAEventActor final : public FWAEventActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = ActorEvent)
		FName EventName;

	UPROPERTY(EditAnywhere, Category = ActorEvent)
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWAEventActor(): EventName(NAME_None) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Global Event")
struct TOROUTILITIES_API FWAEventGlobal final : public FWAEventActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = GlobalEvent)
		FGameplayTag EventID;

	UPROPERTY(EditAnywhere, Category = GlobalEvent)
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWAEventActor(): EventName(NAME_None) {}

protected:

	virtual void RunAction() override;
};