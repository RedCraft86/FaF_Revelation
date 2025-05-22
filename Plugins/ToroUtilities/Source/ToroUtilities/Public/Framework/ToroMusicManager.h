// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "GameFramework/PlayerState.h"
#include "ToroMusicManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroMusicManager : public APlayerState
{
	GENERATED_BODY()

public:

	AToroMusicManager();

	PLAYER_CLASS_GETTER(AToroMusicManager, GetPlayerState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
