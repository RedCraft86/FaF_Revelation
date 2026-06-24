// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "Components/ActorComponent.h"
#include "GameStageManager.generated.h"

class UGameStageNode;

UCLASS(NotBlueprintable, BlueprintType)
class UGameStageManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGameStageManager();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = ContextObject, DisplayName = "Get Stage Manager"))
		static UGameStageManager* Get(const UObject* ContextObject);

	UE5Coro::TCoroutine<> LoadGameStage(const UGameStageNode* InStage);

	bool IsTransitioning() const { return bTransition; }
	const UGameStageNode* GetActiveStage() const { return ActiveStage.Get(); }

private:

	UPROPERTY(Transient)
		bool bTransition;

	UPROPERTY(Transient)
		TWeakObjectPtr<const UGameStageNode> ActiveStage;

	UPROPERTY(Transient)
		TArray<TSoftObjectPtr<UWorld>> LoadedLevels;

	UE5Coro::TCoroutine<> LoadLevel(const TSoftObjectPtr<UWorld>& Level, const bool bVisible);
};
