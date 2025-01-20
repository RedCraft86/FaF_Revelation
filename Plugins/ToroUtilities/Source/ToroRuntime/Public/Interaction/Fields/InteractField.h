// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroVolume.h"
#include "Interaction/InteractionInterface.h"
#include "WorldActions/WorldActionComponent.h"
#include "InteractField.generated.h"

class AToroPlayerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToroFieldEvent, AToroPlayerBase*, Player);
DECLARE_MULTICAST_DELEGATE_OneParam(FToroFieldEventNative, AToroPlayerBase*);

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API AInteractField final : public AToroVolume, public IInteractionInterface
{
	GENERATED_BODY()
	
public:

	AInteractField();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> WorldEventComponent;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		bool bSingleUse;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Settings, meta = (DisplayPriority = -1))
		FInteractionInfo Interaction;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Interacted")
		FToroFieldEvent OnInteractedEvent;
	FToroFieldEventNative OnInteracted;

private:
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override;
	virtual void OnBeginInteract_Implementation(AToroPlayerBase* Player, const FHitResult& HitResult) override;
};
