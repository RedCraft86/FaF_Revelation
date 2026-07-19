// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"

class UStateTreeAIComponent;

UCLASS(NotBlueprintable, BlueprintType)
class AEnemyAIController final : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	UStateTreeAIComponent* GetLogicBrain() const { return LogicBrain; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UStateTreeAIComponent> LogicBrain;

	virtual void OnUnPossess() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
