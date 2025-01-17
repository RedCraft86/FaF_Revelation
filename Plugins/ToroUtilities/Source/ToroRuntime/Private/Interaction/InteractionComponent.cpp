// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/InteractionComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroWidgetManager.h"

UInteractionComponent::UInteractionComponent() : bEnabled(false), HoldTime(0.0f), bInteracting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void UInteractionComponent::SetEnabled(const bool bInEnabled)
{
	
	// TODO: Manage widget
	// if (!Widget)
	// {
	// 	bEnabled = false;
	// 	SetComponentTickEnabled(false);
	// 	SetInteracting(false);
	// }
	
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		SetComponentTickEnabled(bInEnabled);
		// TODO: Manage widget
		if (!bInEnabled ) SetInteracting(false);
	}
}

void UInteractionComponent::SetInteracting(const bool bInInteracting)
{
	if (bInteracting != bInInteracting)
	{
		bInteracting = bInInteracting;
		if (!bInteracting)
		{
			CleanupInteraction();
		}
	}
}

void UInteractionComponent::CleanupInteraction()
{
	if (InteractCache.Target)
	{
		if (InteractCache.bTriggered) IInteraction::EndInteract(InteractCache.Target, Player);
		InteractCache.Reset();
	}
}

void UInteractionComponent::HandleInteractionTick(const float DeltaTime, const FHitResult& HitResult, const FInteractionInfo& InteractResult)
{
	if (InteractCache.Target != HitResult.GetActor())
	{
		if (InteractCache.bTriggered) IInteraction::EndInteract(InteractCache.Target, Player);
		const bool bHeldInteraction = InteractResult.Interaction == EInteractableType::Held;
		HoldTime = bHeldInteraction ? InteractResult.HoldingTime : 0.0f;
		InteractCache.Target = HitResult.GetActor();
		InteractCache.Info = InteractResult;
		if (bHeldInteraction)
		{
			InteractCache.bTriggered = false;
			InteractCache.Progress = 0.0f;
		}
		else
		{
			InteractCache.Progress = 1.0f;
			InteractCache.bTriggered = true;
			IInteraction::BeginInteract(InteractCache.Target, Player, HitResult);
		}
	}

	if (HoldTime >= 0.0f)
	{
		HoldTime -= DeltaTime;
		InteractCache.Progress = FMath::GetMappedRangeValueClamped(
			FVector2D{0.0f, InteractCache.Info.HoldingTime},
			FVector2D{1.0f, 0.0f}, HoldTime);

		if (HoldTime < 0.0f)
		{
			InteractCache.Progress = 1.0f;
			InteractCache.bTriggered = true;
			IInteraction::BeginInteract(InteractCache.Target, Player, HitResult);
		}
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!InteractionLogic.IsBound()) return;
	const FHitResult HitResult = InteractionLogic.Execute();
	if (!HitResult.IsValidBlockingHit()) return;
	
	FInteractionInfo InteractInfo; 
	if (bInteracting && IInteraction::GetInteractionInfo(
		HitResult.GetActor(), HitResult, InteractInfo))
	{
		HandleInteractionTick(DeltaTime, HitResult, InteractInfo);
	}
	else
	{
		CleanupInteraction();
	}
	
	// TODO: Manage widget
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = AToroPlayerCharacter::Get(this);
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		// TODO: Manage widget
	}
}
