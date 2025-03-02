// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Characters/ToroPlayerBase.h"
#include "ToroSideScrollerPlayer.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AToroSideScrollerPlayer : public AToroPlayerBase
{
	GENERATED_BODY()

public:
	
	AToroSideScrollerPlayer();

protected:

	// Slow-TODO
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
