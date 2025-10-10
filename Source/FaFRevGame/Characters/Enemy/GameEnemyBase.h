// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "GameEnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None,
	Roaming,
	Investigating,
	Stalking,
	Chasing,
	Searching
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEnemyState, EEnemyState::None, EEnemyState::Searching)

UCLASS(Abstract)
class FAFREVGAME_API AGameEnemyBase : public AToroCharacter
{
	GENERATED_BODY()

public:

	AGameEnemyBase();

	UFUNCTION(BlueprintCallable, Category = Enemy)
		void SetEnemyState(const EEnemyState InState);

	UFUNCTION(BlueprintPure, Category = Enemy)
		EEnemyState GetEnemyState() const { return EnemyState; }

protected:

	UPROPERTY(VisibleAnywhere, Category = Settings, AdvancedDisplay)
		EEnemyState EnemyState;

	virtual void OnEnemyStateChanged();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
