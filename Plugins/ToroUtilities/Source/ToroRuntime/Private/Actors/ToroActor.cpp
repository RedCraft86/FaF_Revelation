// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroActor.h"

AToroActor::AToroActor() : bEnabled(true), RuntimeGuid(FGuid::NewGuid())
	, bStartWithCollisionEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	SetCanBeDamaged(false);
}

void AToroActor::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		ON_ENABLE_STATE_CHANGED
	}
}

void AToroActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(bStartWithCollisionEnabled);
	if (!bEnabled) OnEnableStateChanged(bEnabled);
}

void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!RuntimeGuid.IsValid())
	{
		RuntimeGuid = FGuid::NewGuid();
	}
}

void AToroActor::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bIsEnabled);
}