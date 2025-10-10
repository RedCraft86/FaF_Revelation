// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API APlayerCharacter final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

protected:

	virtual void SlowTick() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
