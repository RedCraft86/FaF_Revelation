// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "Helpers/GameplayTagMacros.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroPlayerCharacter.generated.h"

namespace PlayerLockTags
{
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(PlayerLock)
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(MainMenu)
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(Startup)
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(Loading)
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(Dialogue)
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(Cinematic)
	TOROFRAMEWORK_API DECLARE_GAMEPLAY_TAG(GameOver)

	TOROFRAMEWORK_API CREATE_TAG_VERIFIER(PlayerLock)
}

UCLASS()
class TOROFRAMEWORK_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	PLAYER_CLASS_GETTER(AToroPlayerCharacter, GetPlayerPawn);
};
