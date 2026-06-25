// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "Interaction/IInteractable.h"
#include "WorldActions/WorldActionList.h"
#include "InteractField.generated.h"

class AToroCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFieldEventDelegateBP, AToroCharacter*, Interactor);
DECLARE_MULTICAST_DELEGATE_OneParam(FFieldEventDelegate, AToroCharacter*);

UCLASS(NotBlueprintable, BlueprintType)
class AInteractField final : public AToroVolume, public IInteractable
{
	GENERATED_BODY()

public:

	AInteractField();

	UPROPERTY(BlueprintAssignable, DisplayName = "On Interaction")
		FFieldEventDelegateBP OnInteractionBP;

	FFieldEventDelegate OnInteraction;

private:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		FText LabelText;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -1))
		bool bSingleUse;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = 1))
		FWorldActionList Actions;

	virtual void BeginPlay() override;
	virtual bool GetInteractionInfo_Implementation(FText& Label) override;
	virtual void BeginInteract_Implementation(AToroCharacter* Interactor) override;
};
