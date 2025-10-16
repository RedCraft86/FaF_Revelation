// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "MusicSystem/WorldMusicManager.h"
#include "Player/PlayerCharacter.h"
#include "EnemyManager.generated.h"

class AGameEnemyBase;

UCLASS()
class FAFREVGAME_API UEnemyManager final : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UEnemyManager() {}

	WORLD_SUBSYSTEM_GETTER(UEnemyManager)

	static void RegisterEnemy(AGameEnemyBase* InEnemy);
	static void UnregisterEnemy(AGameEnemyBase* InEnemy);
	static void UpdateEnemyStatus(const UObject* ContextObject);

private:

	FTimerHandle UpdateTimer;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UWorldMusicManager> MusicManager;
	TSet<TWeakObjectPtr<AGameEnemyBase>> Enemies;

	void UpdateMusicState();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
};
