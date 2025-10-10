// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameEnemyBase.h"
#include "EnemyManager.h"

AGameEnemyBase::AGameEnemyBase(): EnemyState(EEnemyState::None)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameEnemyBase::SetEnemyState(const EEnemyState InState)
{
	if (EnemyState != InState)
	{
		EnemyState = InState;
		OnEnemyStateChanged();
	}
}

void AGameEnemyBase::OnEnemyStateChanged()
{
	UEnemyManager::UpdateEnemyStatus(this);
}

void AGameEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	UEnemyManager::RegisterEnemy(this);
}

void AGameEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
