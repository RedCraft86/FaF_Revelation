// Copyright (C) RedCraft86. All Rights Reserved.

#include "EnemyManager.h"
#include "GameEnemyBase.h"
#include "ToroRuntime.h"
#include "FaFRevGame/FaFRevSettings.h"

void UEnemyManager::RegisterEnemy(AGameEnemyBase* InEnemy)
{
	if (UEnemyManager* Manager = UEnemyManager::Get(InEnemy))
	{
		Manager->Enemies.Add(InEnemy);
		Manager->OnEnemyStatusChanged();
	}
}

void UEnemyManager::UpdateEnemyStatus(const UObject* ContextObject)
{
	if (UEnemyManager* Manager = UEnemyManager::Get(ContextObject))
	{
		Manager->OnEnemyStatusChanged();
	}
}

void UEnemyManager::OnEnemyStatusChanged()
{
	const TMap<EEnemyState, uint8>& StateMapping = UFaFRevSettings::Get()->EnemyToThemeState;

	int32 HighestState = StateMapping.FindRef(EEnemyState::None);
	for (auto It = Enemies.CreateIterator(); It; ++It)
	{
		if (TWeakObjectPtr<AGameEnemyBase>& Enemy = *It; Enemy.IsValid())
		{
			const int32 EnemyState = StateMapping.FindRef(Enemy.Get()->GetEnemyState());
			HighestState = FMath::Max(HighestState, EnemyState);
		}
		else
		{
			It.RemoveCurrent();
		}
	}

	if (MusicManager)
	{
		MusicManager->SetThemeState(HighestState);
	}
}

void UEnemyManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	InWorld.GetTimerManager().SetTimerForNextTick([this]()
	{
		MusicManager = UWorldMusicManager::Get(this);
		OnEnemyStatusChanged();
	});
}

bool UEnemyManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsOnMap(Outer, EToroMapType::Gameplay);
}

bool UEnemyManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
}
