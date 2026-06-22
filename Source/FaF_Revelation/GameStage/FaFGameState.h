// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UE5Coro.h"
#include "Framework/ToroGameState.h"
#include "FaFGameState.generated.h"

class UGameStageData;
class UObjectiveManager;

UCLASS(NotBlueprintable, BlueprintType)
class AFaFGameState final : public AToroGameState
{
	GENERATED_BODY()

public:

	AFaFGameState();

	UE5Coro::TCoroutine<> LoadGameStage(const UGameStageData* InStage);

	UObjectiveManager* GetObjectiveManager() const { return ObjectiveManager; }

private:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UObjectiveManager> ObjectiveManager;

	TWeakObjectPtr<const UGameStageData> CurrentStage;
	TArray<TSoftObjectPtr<UWorld>> LoadedLevels;

	UE5Coro::TCoroutine<> LoadLevel(const TSoftObjectPtr<UWorld>& Level, const bool bVisible);

	void FinishLoadingGame() const;
	virtual void BeginPlay() override;
};
