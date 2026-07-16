// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerHUD.h"
#include "GamePlayerHUD.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class AGamePlayerHUD final : public AToroPlayerHUD
{
	GENERATED_BODY()

public:

	AGamePlayerHUD();

private:

	virtual void BeginPlay() override;
};
