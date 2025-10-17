// Copyright (C) RedCraft86. All Rights Reserved.

#include "InteractField.h"

AInteractField::AInteractField(): bSingleUse(true)
{
	PrimaryActorTick.bCanEverTick = false;

	ActionManager = CreateDefaultSubobject<UWorldActionManager>("ActionManager");
	ActionManager->bAutoConstruction = false;
}

void AInteractField::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	if (IsEnabled())
	{
		ActionManager->SetActions(Actions, false);
		ActionManager->RunActions();

		Super::OnBeginInteract_Implementation(Player, Hit);

		if (bSingleUse)
		{
			SetEnabled(false);
		}
	}
}

#if WITH_EDITOR
void AInteractField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionManager->SetActions(Actions, true);

	bDisplayShadedVolume = true;
	BrushColor = (FLinearColor(0.0f, 0.8f, 0.4f, 1.0f).Desaturate(0.3f) * 0.6f).ToFColor(true);
}
#endif