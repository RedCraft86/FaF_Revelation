// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCharacter.h"
#include "GameEnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None	= 0,
	Roam	= 1,
	Seek	= 2,
	Chase	= 3,
	Search	= 4
};

UCLASS()
class GAMERUNTIME_API AGameEnemyBase : public AToroCharacter
{
	GENERATED_BODY()

public:

	AGameEnemyBase() : EnemyState(EEnemyState::None) {}

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void SetEnemyState(const EEnemyState InState) { EnemyState = InState; }

	UFUNCTION(BlueprintPure, Category = Game)
		EEnemyState GetEnemyState() const { return EnemyState; }

private:

	UPROPERTY() EEnemyState EnemyState;
};
