﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroVolume.h"
#include "Interaction/InteractionInterface.h"
#include "WorldActions/WorldActionComponent.h"
#include "InteractField.generated.h"

class AToroPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToroFieldEvent, AToroPlayerCharacter*, Player);
DECLARE_MULTICAST_DELEGATE_OneParam(FToroFieldEventNative, AToroPlayerCharacter*);

UCLASS(NotBlueprintable, BlueprintType)
class TOROUTILITIES_API AInteractField : public AToroVolume, public IInteractionInterface
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

protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override { return Interaction; }
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) override;
};