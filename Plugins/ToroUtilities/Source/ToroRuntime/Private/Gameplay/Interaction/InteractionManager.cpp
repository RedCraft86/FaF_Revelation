// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractionManager.h"
#include "Interaction/InteractionInterface.h"

UInteractionManager::UInteractionManager(): bEnabled(false), bInteracting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void UInteractionManager::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		if (!bEnabled && bInteracting)
		{
			SetInteracting(false);
		}
		else
		{
			// UpdateWidget();
		}
	}
}

void UInteractionManager::SetInteracting(const bool bInInteracting)
{
	const bool bTarget = bInInteracting && CanInteract();
	if (bInteracting != bTarget)
	{
		bInteracting = bTarget;
		if (!bInteracting)
		{
			CleanupCache();
		}
	}
}

void UInteractionManager::CleanupCache()
{
	if (InteractCache.Target.IsValid())
	{
		InteractCache.StopInteract(PlayerChar);
		InteractCache.Reset();
		// UpdateWidget();
	}
}

void UInteractionManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = GetOwner<AToroPlayerCharacter>();
}

void UInteractionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);

	if (!PlayerChar) return;
	if (!CanInteract() || !bInteracting)
	{
		CleanupCache();
		return;
	}

	const FHitResult& HitResult = HandleTrace.Execute();
	AActor* HitActor = HitResult.GetActor();

	FInteractionInfo Info;
	if (!IInteractionInterface::GetInteractInfo(HitActor, HitResult, Info))
	{
		CleanupCache();
		return;
	}

	if (InteractCache.Target != HitActor)
	{
		InteractCache.StopInteract(PlayerChar);

		InteractCache.Target = HitActor;
		InteractCache.StartInteract(PlayerChar, HitResult);
		// UpdateWidget();
	}
}
