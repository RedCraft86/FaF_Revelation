// Copyright (C) RedCraft86. All Rights Reserved.

#include "InteractionComponent.h"
#include "Libraries/ToroMathLibrary.h"
#include "Player/GamePlayerChar.h"

#define ON_UPDATE() if (OnUpdate.IsBound()) OnUpdate.Execute(IsEnabled(), InteractCache)

UInteractionComponent::UInteractionComponent()
	: ReachDistance(250.0f), InteractTrace(ECC_Visibility), bInteracting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

UInteractionComponent* UInteractionComponent::Get(const UObject* ContextObject)
{
	AGamePlayerChar* Player = AGamePlayerChar::Get<AGamePlayerChar>(ContextObject);
	return Player ? Player->Interaction : nullptr;
}

bool UInteractionComponent::IsEnabled() const
{
	return Player && !Player->IsLocked() && Player->HasControlFlag(PCF_CanInteract);
}

void UInteractionComponent::SetInteracting(const bool bInInteracting)
{
	if (bInteracting != bInInteracting)
	{
		bInteracting = bInInteracting && IsEnabled();
		if (!bInteracting)
		{
			CleanupInteractionData();
		}
	}
}

FHitResult UInteractionComponent::Interact() const
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

void UInteractionComponent::CleanupInteractionData()
{
	if (InteractCache.Target)
	{
		if (InteractCache.bTriggered)
		{
			IInteractionInterface::EndInteract(InteractCache.Target, Player);
		}
		InteractCache.Reset();
		ON_UPDATE();
	}
}

void UInteractionComponent::HandleInteractionTick(const FHitResult& HitResult, const FInteractionInfo& Interaction)
{
	if (InteractCache.Target != HitResult.GetActor())
	{
		if (InteractCache.bTriggered)
		{
			IInteractionInterface::EndInteract(InteractCache.Target, Player);
		}
		InteractCache.Target = HitResult.GetActor();
		InteractCache.Info = Interaction;
		InteractCache.bTriggered = true;
		IInteractionInterface::BeginInteract(InteractCache.Target, Player, HitResult);
		ON_UPDATE();
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = GetOwner<AGamePlayerChar>();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (const FHitResult HitResult = Interact(); HitResult.IsValidBlockingHit())
	{
		FInteractionInfo InteractInfo; 
		if (IsEnabled() && bInteracting && IInteractionInterface::GetInteractionInfo(
			HitResult.GetActor(), HitResult, InteractInfo))
		{
			HandleInteractionTick(HitResult, InteractInfo);
		}
		else
		{
			CleanupInteractionData();
		}
	}
}
