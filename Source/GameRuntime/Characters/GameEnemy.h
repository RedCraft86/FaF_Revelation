// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "Components/StateTreeComponent.h"
#include "GameEnemy.generated.h"

class UStateTreeAIComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class AGameEnemy : public AToroCharacter
{
	GENERATED_BODY()

	friend class AEnemyAIController;

public:

	AGameEnemy();

	UStateTreeAIComponent* GetLogicBrain() const;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Schema = "/Script/GameplayStateTreeModule.StateTreeAIComponentSchema", SchemaCanBeOverriden))
		FStateTreeReference LogicTree;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
