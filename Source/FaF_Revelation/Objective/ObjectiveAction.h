// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "ObjectiveManager.h"
#include "GameplayTagContainer.h"
#include "WorldActions/WorldActionBase.h"
#include "ObjectiveAction.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UWorldAction_Objective final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "Objective"))
		FGameplayTag Objective;

private:

	virtual void OnExecute_Implementation() override
	{
		if (Objective.IsValid())
		{
			if (UObjectiveManager* Manager = UObjectiveManager::Get(this))
			{
				Manager->CompleteObjective(Objective);
			}
		}
	}
};
