// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractField.h"
#include "TriggerField.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API ATriggerField final : public AToroVolume
{
	GENERATED_BODY()
	
public:

	ATriggerField();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UWorldActionComponent> WorldEventComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (DisplayPriority = -1))
		bool bSingleUse;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Triggered")
		FToroFieldEvent OnTriggeredEvent;
	FToroFieldEventNative OnTriggered;

private:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
