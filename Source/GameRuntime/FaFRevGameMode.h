// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameState/GlobalState.h"
#include "Framework/ToroGameMode.h"
#include "GameState/SessionState.h"
#include "GamePlayer/GamePlayerCharacter.h"
#include "GamePlayer/GamePlayerController.h"
#include "FaFRevGameMode.generated.h"

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class AFaFRevGameMode final : public AToroGameMode
{
	GENERATED_BODY()

public:

	AFaFRevGameMode()
	{
		GameStateClass = AGlobalState::StaticClass();
		PlayerControllerClass = AGamePlayerController::StaticClass();
		DefaultPawnClass = AGamePlayerCharacter::StaticClass();
		PlayerStateClass = ASessionState::StaticClass();
	}
};
