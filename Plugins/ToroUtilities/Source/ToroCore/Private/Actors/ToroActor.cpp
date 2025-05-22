// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/ToroActor.h"

AToroActor::AToroActor(): bEnabled(true), RuntimeGuid(FGuid::NewGuid()), bStartWithCollision(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	DEFINE_ACTOR_ICON
#endif

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
	SetActorEnableCollision(bStartWithCollision);
	if (!bEnabled) ON_ENABLE_STATE_CHANGED
}

void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!RuntimeGuid.IsValid()) { RuntimeGuid = FGuid::NewGuid(); }
#if WITH_EDITORONLY_DATA
	UPDATE_ACTOR_ICON
#endif
}

void AToroActor::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bIsEnabled);
}
