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

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
