// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "FaFRevSettings.h"
#include "Player/PlayerCharacter.h"
#include "Subsystems/WorldSubsystem.h"
#include "MusicSystem/WorldMusicManager.h"
#include "EnemyManager.generated.h"

class AGameEnemyBase;

UCLASS()
class FAFREVGAME_API UEnemyManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	UEnemyManager(): TickTime(0.0f) {}

	WORLD_SUBSYSTEM_GETTER(UEnemyManager)

	static void RegisterEnemy(AGameEnemyBase* InEnemy);
	static void UnregisterEnemy(AGameEnemyBase* InEnemy);
	static void UpdateEnemyStatus(const UObject* ContextObject);

private:

	float TickTime;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UWorldMusicManager> MusicManager;
	TObjectPtr<const UFaFRevSettings> FaFRevSettings;
	TSet<TWeakObjectPtr<AGameEnemyBase>> Enemies;

	void UpdateMusicState();

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
};
