// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/InteractionComponent.h"
#include "Framework/ToroPlayerCharacter.h"

#define UpdateUI() if (OnDataUpdate.IsBound()) OnDataUpdate.Execute(CanInteract(), InteractCache)

UInteractionComponent::UInteractionComponent(): bEnabled(true), bInteracting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void UInteractionComponent::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		if (!bEnabled) SetInteracting(false, nullptr);
	}
}

void UInteractionComponent::SetInteracting(const bool bInInteracting, AToroPlayerCharacter* InPlayer)
{
	bool bTarget = bInInteracting && InPlayer;
	if (bInteracting != bTarget)
	{
		bInteracting = bTarget;
		if (bInteracting)
		{
			Player = InPlayer;
		}
		else
		{
			CleanupCache();
		}
	}
}

void UInteractionComponent::CleanupCache()
{
	if (InteractCache.Target)
	{
		InteractCache.StopInteract(Player);
		InteractCache.Reset();
		UpdateUI();
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);

	if (!Player) return;
	if (!CanInteract() || !bInteracting)
	{
		CleanupCache();
		return;
	}

	FInteractionInfo Info;
	const FHitResult& HitResult = HandleTrace.Execute();
	AActor* HitActor = HitResult.GetActor();
	if (!IInteractionInterface::GetInteractInfo(HitActor, HitResult, Info))
	{
		CleanupCache();
		return;
	}

	if (InteractCache.Target != HitActor)
	{
		// End existing if changing targets
		InteractCache.StopInteract(Player);
					
		InteractCache.Info = Info;
		InteractCache.Target = HitActor;
		InteractCache.StartInteract(Player, HitResult);
		UpdateUI();
	}
}
