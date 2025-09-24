// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetterMacros.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();
	
	GLOBAL_CLASS_GETTER(AToroGameState, GetGameState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UWorldMusicManager> MusicManager;

	// Technically, this is player specific but that's really only for Multiplayer
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UAchievementManager> Achievements;
};
