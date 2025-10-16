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
		UEnemyManager::UpdateEnemyStatus(this);
	}
}

bool AGameEnemyBase::IsEnemyState(const EEnemyState InState) const
{
	return EnemyState == InState;
}

EEnemyState AGameEnemyBase::GetEnemyState() const
{
	return EnemyState;
}
}

void AGameEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	UEnemyManager::RegisterEnemy(this);
}

void AGameEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEnemyManager::UnregisterEnemy(this);
	Super::EndPlay(EndPlayReason);
}

void AGameEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
