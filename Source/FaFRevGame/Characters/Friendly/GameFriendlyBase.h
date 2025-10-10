// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "GameFriendlyBase.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API AGameFriendlyBase : public AToroCharacter
{
	GENERATED_BODY()

public:

	AGameFriendlyBase();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
