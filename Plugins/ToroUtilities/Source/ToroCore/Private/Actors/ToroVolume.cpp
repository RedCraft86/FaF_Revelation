// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroVolume.h"
#include "Components/BrushComponent.h"

AToroVolume::AToroVolume() : bEnabled(true), RuntimeGuid(FGuid::NewGuid())
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

#if WITH_EDITORONLY_DATA
	CREATE_DEBUG_ICON(DebugIcon)
#endif

	SetCanBeDamaged(false);
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
#if WITH_EDITORONLY_DATA
	UPDATE_DEBUG_ICON(DebugIcon);
#endif
	if (!RuntimeGuid.IsValid())
	{
		RuntimeGuid = FGuid::NewGuid();
	}
}

void AToroVolume::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
}
