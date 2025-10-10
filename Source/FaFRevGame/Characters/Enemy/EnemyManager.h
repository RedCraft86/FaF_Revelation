// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "MusicSystem/WorldMusicManager.h"
#include "EnemyManager.generated.h"

UCLASS()
class FAFREVGAME_API UEnemyManager final : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UEnemyManager() {}

	WORLD_SUBSYSTEM_GETTER(UEnemyManager)

	static void RegisterEnemy(class AGameEnemyBase* InEnemy);
	static void UpdateEnemyStatus(const UObject* ContextObject);

private:

	TObjectPtr<UWorldMusicManager> MusicManager;
	TSet<TWeakObjectPtr<AGameEnemyBase>> Enemies;

	void OnEnemyStatusChanged();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
};
