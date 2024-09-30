// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "HeartBeatGameWidget.h"
#include "Components/ActorComponent.h"
#include "HeartBeatGame.generated.h"

UCLASS(ClassGroup = (Games), meta = (BlueprintSpawnableComponent))
class FAF_REV_API UHeartBeatGame : public UActorComponent
{
	GENERATED_BODY()

public:

	UHeartBeatGame();

protected:

	UPROPERTY(Transient) TObjectPtr<USceneComponent> LookComp;
	UPROPERTY(Transient) TObjectPtr<UHeartBeatGameWidget> Widget;
	UPROPERTY(Transient) TObjectPtr<class AFRPlayerBase> PlayerChar;

	virtual void BeginPlay() override;
};
