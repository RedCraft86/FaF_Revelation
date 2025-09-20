// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroPlayerState.generated.h"

UCLASS(meta = (ChildCanTick = true))
class TOROFRAMEWORK_API AToroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AToroPlayerState();
	
	PLAYER_CLASS_GETTER(AToroPlayerState, GetPlayerState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
