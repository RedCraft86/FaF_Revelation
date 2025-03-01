// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroNPCBase.h"
#include "Characters/ToroPlayerBase.h"

AToroNPCBase::AToroNPCBase() : bEnemy(false), EnemyState(EToroEnemyState::None)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToroNPCBase::SetNPCState(const EToroEnemyState InState)
{
	if (!bEnemy) return;
	if (EnemyState != InState)
	{
		EnemyState = InState;
		if (PlayerChar) PlayerChar->UpdateEnemy(this);
	}
}

void AToroNPCBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = AToroPlayerBase::Get(this);
}
