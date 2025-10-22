// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Dialogue.h"
#include "InteractableNPC.h"
#include "Narrative/NarrativeManager.h"
#include "SimpleDialogueNPC.generated.h"

UCLASS()
class FAFREVGAME_API ASimpleDialogueNPC final : public AInteractableNPC
{
	GENERATED_BODY()

	ASimpleDialogueNPC() {}

	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit) override
	{
		if (Narrative) Narrative->BeginDialogue(Dialogue);
		Super::OnBeginInteract_Implementation(Player, Hit);
	}

protected:

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UDialogue> Dialogue;

	TObjectPtr<UNarrativeManager> Narrative;

	virtual	void BeginPlay() override
	{
		Super::BeginPlay();
		GetWorldTimerManager().SetTimerForNextTick([this]()
		{
			Narrative = UNarrativeManager::Get(this);
		});
	}
};
