// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerController.h"
#include "GamePlayerController.generated.h"

UCLASS()
class FAFREVGAME_API AGamePlayerController final : public AToroPlayerController
{
	GENERATED_BODY()

	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
};
