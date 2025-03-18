// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroVolume.h"
#include "InteractField.h"
#include "TriggerField.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TOROUTILITIES_API ATriggerField : public AToroVolume
{
	GENERATED_BODY()
	
public:

	ATriggerField();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> WorldEventComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (DisplayPriority = -1))
	bool bSingleUse;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Triggered")
	FToroFieldEvent OnTriggeredEvent;
	FToroFieldEventNative OnTriggered;

protected:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};