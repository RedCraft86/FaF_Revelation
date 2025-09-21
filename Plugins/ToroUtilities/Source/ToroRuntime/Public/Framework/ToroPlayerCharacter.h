// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "Helpers/GameplayTagMacros.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroPlayerCharacter.generated.h"

namespace PlayerLockTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(PlayerLock)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(MainMenu)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Startup)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Loading)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Dialogue)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Cinematic)
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(GameOver)

	TORORUNTIME_API CREATE_TAG_VERIFIER(PlayerLock)
}

UCLASS()
class TORORUNTIME_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	PLAYER_CLASS_GETTER(AToroPlayerCharacter, GetPlayerPawn);
};
