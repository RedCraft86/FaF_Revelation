// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "EnemyAIController.h"
#include "Characters/GameEnemy.h"
#include "Components/StateTreeAIComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	LogicBrain = CreateDefaultSubobject<UStateTreeAIComponent>("LogicBrain");
	LogicBrain->SetStartLogicAutomatically(false);
}

void AEnemyAIController::OnUnPossess()
{
	if (LogicBrain->IsRunning())
	{
		LogicBrain->StopLogic(TEXT("Pawn Unpossessed"));
	}
	Super::OnUnPossess();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (const AGameEnemy* EnemyPawn = Cast<AGameEnemy>(InPawn))
	{
		if (EnemyPawn->LogicTree.IsValid())
		{
			LogicBrain->SetStateTreeReference(EnemyPawn->LogicTree);
			LogicBrain->StartLogic();
		}
	}
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (LogicBrain->IsRunning())
	{
		LogicBrain->StopLogic(TEXT("AIController EndPlay"));
	}
	Super::EndPlay(EndPlayReason);
}
