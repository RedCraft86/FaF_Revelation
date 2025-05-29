// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "EventActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Event Actions", meta = (Hidden))
struct TOROUTILITIES_API FWTaskEventActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Remove Event")
struct TOROUTILITIES_API FWTaskEventRemote final : public FWTaskEventActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = RemoteEvent)
		FName EventName;

	FWTaskEventRemote(): EventName(NAME_None) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Actor Event")
struct TOROUTILITIES_API FWTaskEventActor final : public FWTaskEventActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ActorEvent)
		FName EventName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ActorEvent)
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWTaskEventActor(): EventName(NAME_None) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};