// Copyright (C) RedCraft86. All Rights Reserved.

#include "EnemyManager.h"
#include "GameEnemyBase.h"
#include "FaFRevGame/FaFRevSettings.h"
#include "ToroRuntime.h"

void UEnemyManager::RegisterEnemy(AGameEnemyBase* InEnemy)
{
	if (!InEnemy) return;
	if (UEnemyManager* Manager = UEnemyManager::Get(InEnemy))
	{
		Manager->Enemies.Add(InEnemy);
		Manager->UpdateMusicState();
	}
}

void UEnemyManager::UnregisterEnemy(AGameEnemyBase* InEnemy)
{
	if (!InEnemy) return;
	if (UEnemyManager* Manager = UEnemyManager::Get(InEnemy))
	{
		Manager->Enemies.Remove(InEnemy);
		Manager->UpdateMusicState();
	}
}

void UEnemyManager::UpdateEnemyStatus(const UObject* ContextObject)
{
	if (UEnemyManager* Manager = UEnemyManager::Get(ContextObject))
	{
		Manager->UpdateMusicState();
	}
}

void UEnemyManager::UpdateMusicState()
{
	uint8 HighestState = 0;
	if (Player)
	{
		const FVector PlayerLoc = Player->GetActorLocation();
		const UFaFRevSettings* Settings = UFaFRevSettings::Get();
		const float MaxDistSq = Settings->EnemyThemeRadius * Settings->EnemyThemeRadius;
		for (auto It = Enemies.CreateIterator(); It; ++It)
		{
			if (TWeakObjectPtr<AGameEnemyBase>& WeakEnemy = *It; WeakEnemy.IsValid())
			{
				const AGameEnemyBase* Enemy = WeakEnemy.Get();
				if (FVector::DistSquared(Enemy->GetActorLocation(), PlayerLoc) > MaxDistSq)
				{
					continue;
				}

				const uint8 EnemyState = static_cast<uint8>(Enemy->GetEnemyState());
				if (EnemyState > HighestState)
				{
					HighestState = EnemyState;
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

void UEnemyManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	InWorld.GetTimerManager().SetTimerForNextTick([this]()
	{
		Player = APlayerCharacter::Get<APlayerCharacter>(this);
		MusicManager = UWorldMusicManager::Get(this);
	});
	InWorld.GetTimerManager().SetTimer(UpdateTimer, this, &UEnemyManager::UpdateMusicState,
		UFaFRevSettings::Get()->EnemyThemeUpdateInterval, true, 0.1f);
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
