// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameInteraction.h"
#include "Player/GamePlayer.h"
#include "Camera/CameraComponent.h"
#include "Libraries/ToroMathLibrary.h"

UGameInteraction::UGameInteraction()
	: ReachDistance(250.0f), InteractTrace(ECC_Visibility), bEnabled(false), HoldTime(0.0f), bInteracting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void UGameInteraction::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		SetComponentTickEnabled(bInEnabled);
		if (!bInEnabled) SetInteracting(false);
	}
}

void UGameInteraction::SetInteracting(const bool bInInteracting)
{
	if (bInteracting != bInInteracting)
	{
		bInteracting = bInInteracting;
		if (!bInteracting)
		{
			CleanupInteractionData();
		}
	}
}

FHitResult UGameInteraction::Interact() const
{
	FHitResult Hit;
	if (Player)
	{
		FVector Start, End;
		UToroMathLibrary::GetComponentLineTraceVectors(Player->PlayerCamera,
			EVectorDirection::Forward, ReachDistance, Start, End);

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, InteractTrace, 
			FCollisionQueryParams(NAME_None, true, GetOwner()));
	}

	return Hit;
}

void UGameInteraction::CleanupInteractionData()
{
	if (InteractCache.Target)
	{
		if (InteractCache.bTriggered) IInteractionInterface::EndInteract(InteractCache.Target, Player);
		InteractCache.Reset();
	}
}

void UGameInteraction::HandleInteractionTick(float DeltaTime, const FHitResult& HitResult, const FInteractionInfo& InteractResult)
{
	if (InteractCache.Target != HitResult.GetActor())
	{
		if (InteractCache.bTriggered) IInteractionInterface::EndInteract(InteractCache.Target, Player);
		const bool bHeldInteraction = InteractResult.Interaction == EInteractType::Held;
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
			IInteractionInterface::BeginInteract(InteractCache.Target, Player, HitResult);
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
			IInteractionInterface::BeginInteract(InteractCache.Target, Player, HitResult);
		}
	}
}

void UGameInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (const FHitResult HitResult = Interact(); HitResult.IsValidBlockingHit())
	{
		FInteractionInfo InteractInfo; 
		if (bInteracting && IInteractionInterface::GetInteractionInfo(
			HitResult.GetActor(), HitResult, InteractInfo))
		{
			HandleInteractionTick(DeltaTime, HitResult, InteractInfo);
		}
		else
		{
			CleanupInteractionData();
		}
	}
}

void UGameInteraction::BeginPlay()
{
	Super::BeginPlay();
	Player = GetOwner<AGamePlayer>();
}
