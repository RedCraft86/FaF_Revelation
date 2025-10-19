// Copyright (C) RedCraft86. All Rights Reserved.

#include "EnemyManager.h"
#include "GameEnemyBase.h"
#include "ToroRuntime.h"

void UEnemyManager::RegisterEnemy(AGameEnemyBase* InEnemy)
{
	if (!InEnemy) return;
	if (UEnemyManager* Manager = UEnemyManager::Get(InEnemy))
	{
		Manager->Enemies.Add(InEnemy);
		Manager->TickTime = 2.0f;
	}
}

void UEnemyManager::UnregisterEnemy(AGameEnemyBase* InEnemy)
{
	if (!InEnemy) return;
	if (UEnemyManager* Manager = UEnemyManager::Get(InEnemy))
	{
		Manager->TickTime = 0.0f;
		Manager->Enemies.Remove(InEnemy);
		Manager->UpdateMusicState();
	}
}

void UEnemyManager::UpdateEnemyStatus(const UObject* ContextObject)
{
	if (UEnemyManager* Manager = UEnemyManager::Get(ContextObject))
	{
		Manager->TickTime = 2.0f;
	}
}

void UEnemyManager::UpdateMusicState()
{
	uint8 HighestState = 0;
	if (Player)
	{
		const FVector PlayerLoc = Player->GetActorLocation();
		const UFaFRevSettings* Settings = UFaFRevSettings::Get();
		const float MaxDistSq = Settings->LowThreatRadius * Settings->LowThreatRadius;
		for (auto It = Enemies.CreateIterator(); It; ++It)
		{
			if (TWeakObjectPtr<AGameEnemyBase>& WeakEnemy = *It; WeakEnemy.IsValid())
			{
				const AGameEnemyBase* Enemy = WeakEnemy.Get();
				const uint8 EnemyState = static_cast<uint8>(Enemy->GetEnemyState());
				if (EnemyState == 0) continue;

				// If Roaming or Stalking, it should be proximity based, otherwise always accept if state is higher
				if (EnemyState > HighestState && (EnemyState > static_cast<uint8>(EEnemyState::Stalking)
					|| FVector::DistSquared(Enemy->GetActorLocation(), PlayerLoc) < MaxDistSq))
				{
					HighestState = EnemyState;
					if (HighestState == static_cast<uint8>(EEnemyState::Chasing))
					{
						break;
					}
				}
			}
			else
			{
				It.RemoveCurrent();
			}
		}
	}

	if (MusicManager)
	{
		MusicManager->SetThemeState(HighestState);
	}
}

bool UEnemyManager::IsTickable() const
{
	return Super::IsTickable() && !Enemies.IsEmpty()
		&& FaFRevSettings && Player && MusicManager;
}

void UEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TickTime > FaFRevSettings->ThemeRefreshRate)
	{
		TickTime = 0.0f;
		UpdateMusicState();
	}
	else TickTime += DeltaTime;
}

void UEnemyManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	FaFRevSettings = UFaFRevSettings::Get();
	InWorld.GetTimerManager().SetTimerForNextTick([this]()
	{
		Player = APlayerCharacter::Get<APlayerCharacter>(this);
		MusicManager = UWorldMusicManager::Get(this);
		UpdateMusicState();
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
