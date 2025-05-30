// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Helpers/ClassGetters.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

UCLASS()
class TOROUTILITIES_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();

	GAMEPLAY_CLASS_GETTER(AToroGameState, GetGameState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
	
protected:

	UPROPERTY() TMap<FGameplayTag, FGameplayTag> CharacterToZone;
};
