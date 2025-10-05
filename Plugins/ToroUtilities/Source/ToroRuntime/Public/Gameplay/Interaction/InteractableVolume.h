// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "InteractionMarker.h"
#include "InteractionInterface.h"
#include "InteractableVolume.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AInteractableVolume final : public AToroVolume, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractableVolume();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionMarker> Marker;

	virtual void SetMarkerState_Implementation(const bool bVisible) override;
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo Interaction;
};
