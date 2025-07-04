﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Framework/ToroGameInstance.h"
#include "FaFRevGameInstance.generated.h"

UCLASS()
class FAFREVGAME_API UFaFRevGameInstance final : public UToroGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY() TOptional<FGuid> PlayFromPhase;
	UPROPERTY() TOptional<FGameplayTag> LastJumpscare;
};
