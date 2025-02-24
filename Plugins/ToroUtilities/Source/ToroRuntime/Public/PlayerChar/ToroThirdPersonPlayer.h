// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Characters/ToroPlayerBase.h"
#include "ToroThirdPersonPlayer.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AToroThirdPersonPlayer : public AToroPlayerBase
{
	GENERATED_BODY()

public:
	
	AToroThirdPersonPlayer();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
