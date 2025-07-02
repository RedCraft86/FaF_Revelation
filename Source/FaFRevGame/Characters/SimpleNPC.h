// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Dialogue.h"
#include "Actors/ToroCharacter.h"
#include "Narrative/ToroNarrative.h"
#include "PlayerChar/GamePlayerChar.h"
#include "Components/CapsuleComponent.h"
#include "Interaction/InteractionInterface.h"
#include "SimpleNPC.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API ASimpleNPC : public AToroCharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:

	ASimpleNPC()
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UDialogue> Dialogue;

	UPROPERTY(EditAnywhere, Category = Settings)
		FInteractionInfo Interaction;

	virtual FInteractionInfo GetInteractInfo_Implementation(const FHitResult& HitResult) override
	{
		return IsHidden() || !Dialogue ? FInteractionInfo::GetEmpty() : Interaction;
	}

	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) override
	{
		if (const AGamePlayerChar* PlayerChar = Cast<AGamePlayerChar>(Player))
		{
			PlayerChar->Narrative->BeginDialogue(Dialogue);
		}
	}

	virtual void OnConstruction(const FTransform& Transform) override
	{
		if (Dialogue == UDialogue::StaticClass()) Dialogue = nullptr;
	}
};
