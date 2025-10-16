// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "GameEnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	// No activity (no intensity)
	None = 0,

	// Enemy roaming (lowest intensity)
	Roaming = 1,

	// Enemy discreetly stalking player (low intensity)
	Stalking = 2,

	// Enemy investigating stimuli (medium intensity)
	Alerted = 3,

	// Enemy searching player after losing (high intensity)
	Hunting = 4,

	// Enemy chasing player (highest intensity)
	Chasing = 5
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEnemyState, EEnemyState::None, EEnemyState::Chasing)

UCLASS(Abstract)
class FAFREVGAME_API AGameEnemyBase : public AToroCharacter
{
	GENERATED_BODY()

public:

	AGameEnemyBase();

	UFUNCTION(BlueprintCallable, Category = Enemy)
		virtual void SetEnemyState(const EEnemyState InState);

	UFUNCTION(BlueprintPure, Category = Enemy)
		virtual bool IsEnemyState(const EEnemyState InState) const;

	UFUNCTION(BlueprintPure, Category = Enemy)
		virtual EEnemyState GetEnemyState() const;

protected:

	UPROPERTY(VisibleAnywhere, Category = Settings, AdvancedDisplay)
		EEnemyState EnemyState;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
};
