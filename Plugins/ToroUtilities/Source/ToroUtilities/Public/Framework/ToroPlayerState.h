// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "GameFramework/PlayerState.h"
#include "ToroPlayerState.generated.h"

UCLASS()
class TOROUTILITIES_API AToroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AToroPlayerState();

	PLAYER_CLASS_GETTER(AToroPlayerState, GetPlayerState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
