// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFriendlyBase.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/InteractionMarker.h"
#include "InteractableNPC.generated.h"

UCLASS()
class FAFREVGAME_API AInteractableNPC : public AGameFriendlyBase, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInteractableNPC();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionMarker> Marker;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractedBP, AToroCharacter*, Target, AToroPlayerCharacter*, Player);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Interacted")
		FOnInteractedBP OnInteractedBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteracted, AToroCharacter*, AToroPlayerCharacter*)
	FOnInteracted OnInteracted;

	virtual void SetMarkerState_Implementation(const bool bVisible) override;
	virtual bool GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo Interaction;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
