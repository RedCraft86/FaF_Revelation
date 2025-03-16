// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroActor.h"

AToroActor::AToroActor() : bEnabled(true), RuntimeGuid(FGuid::NewGuid())
	, bStartWithCollisionEnabled(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	CREATE_DEBUG_ICON(DebugIcon)
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
	SetActorEnableCollision(bStartWithCollisionEnabled);
	if (!bEnabled) ON_ENABLE_STATE_CHANGED
}

void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
	UPDATE_DEBUG_ICON(DebugIcon);
#endif
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