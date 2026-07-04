// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerCharacter.h"
#include "GamePlayerCharacter.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class AGamePlayerCharacter final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	AGamePlayerCharacter();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
