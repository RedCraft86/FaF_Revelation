// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "InteractField.h"
#include "Actors/ToroVolume.h"
#include "WorldActions/WorldActionList.h"
#include "TriggerField.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class ATriggerField final : public AToroVolume
{
	GENERATED_BODY()

public:

	ATriggerField();

	UPROPERTY(BlueprintAssignable, DisplayName = "On Triggered")
		FFieldEventDelegateBP OnTriggeredBP;

	FFieldEventDelegate OnTriggered;

private:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		bool bSingleUse;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = 1))
		FWorldActionList Actions;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
