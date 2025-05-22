// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "Camera/PlayerCameraManager.h"
#include "ToroCameraManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AToroCameraManager();

	PLAYER_CLASS_GETTER(AToroCameraManager, GetPlayerCameraManager)

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
