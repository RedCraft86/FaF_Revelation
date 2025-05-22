// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "GameFramework/GameModeBase.h"
#include "ToroGameMode.generated.h"

UCLASS()
class TOROUTILITIES_API AToroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AToroGameMode();

	GAMEPLAY_CLASS_GETTER(AToroGameMode, GetGameMode)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

protected:

	UPROPERTY(Transient)
	TObjectPtr<class UToroGameInstance> GameInstance;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
