// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "InteractionMarker.h"
#include "InteractionInterface.h"
#include "InteractableActor.generated.h"

UCLASS(Abstract, meta = (ChildCanTick = true))
class TORORUNTIME_API AInteractableActor final : public AToroActor, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionMarker> Marker;

	virtual void SetMarkerState_Implementation(const bool bVisible) override;
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo Interaction;
};
