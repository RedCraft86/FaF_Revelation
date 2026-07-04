// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerController.h"
#include "GamePlayerController.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class AGamePlayerController final : public AToroPlayerController
{
	GENERATED_BODY()

public:

	AGamePlayerController();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
