// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroVolume.h"
#include "Components/BrushComponent.h"

AToroVolume::AToroVolume(): bEnabled(true), RuntimeGuid(FGuid::NewGuid())
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

#if WITH_EDITORONLY_DATA
	DEFINE_ACTOR_ICON
#endif

	SetCanBeDamaged(false);
	GetBrushComponent()->SetCollisionProfileName("Trigger");
	
#if WITH_EDITOR
	bColored = true;
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
#if WITH_EDITORONLY_DATA
	UPDATE_ACTOR_ICON
#endif
}

void AToroVolume::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
}
