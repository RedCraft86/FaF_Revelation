// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroVolume.h"
#include "Components/BrushComponent.h"

AToroVolume::AToroVolume(): bEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

#if WITH_EDITORONLY_DATA
	DEFINE_VISUAL
#endif

	SetCanBeDamaged(false);
	GetBrushComponent()->SetCollisionProfileName("Trigger");
	
#if WITH_EDITOR
	bColored = true;
	BrushColor = FColor(124, 75, 155);
#endif
}

void AToroVolume::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		ON_ENABLE_STATE_CHANGED
	}
}

void AToroVolume::BeginPlay()
{
	Super::BeginPlay();
	if (!bEnabled) ON_ENABLE_STATE_CHANGED
}

void AToroVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!RuntimeGuid.IsValid()) { RuntimeGuid = FGuid::NewGuid(); }
}

void AToroVolume::EnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
}
