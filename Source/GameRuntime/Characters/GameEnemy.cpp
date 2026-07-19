// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameEnemy.h"
#include "EnemyAIController.h"

AGameEnemy::AGameEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAIController::StaticClass();
}

UStateTreeAIComponent* AGameEnemy::GetLogicBrain() const
{
	if (const AEnemyAIController* Cont = GetController<AEnemyAIController>())
	{
		return Cont->GetLogicBrain();
	}
	return nullptr;
}

void AGameEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AGameEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
