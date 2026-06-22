// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ObjectiveActor.generated.h"

UCLASS(Abstract)
class FAF_REVELATION_API AObjectiveActor final : public AActor
{
	GENERATED_BODY()

public:

	AObjectiveActor();

	UFUNCTION(BlueprintNativeEvent, Category = Objective)
		void StartObjective();
	void StartObjective_Implementation() PURE_VIRTUAL(AObjectiveActor::StartObjective,);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Objective)
		void MarkCompleted(const bool bNotifyManager = true);
	void MarkCompleted_Implementation(const bool bNotifyManager);
};
