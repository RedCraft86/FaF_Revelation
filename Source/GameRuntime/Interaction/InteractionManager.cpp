// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InteractionManager.h"
#include "GameFramework/Actor.h"
#include "Actors/ToroCharacter.h"
#include "GamePlayer/GamePlayerCharacter.h"

UInteractionManager::UInteractionManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	PrimaryComponentTick.TickInterval = 0.01f;
	bAutoActivate = false;
}

UInteractionManager* UInteractionManager::Get(const UObject* ContextObject)
{
	const AGamePlayerCharacter* Player = AToroPlayerCharacter::Get<AGamePlayerCharacter>(ContextObject);
	return IsValid(Player) ? Player->GetInteractionManager() : nullptr;
}

void UInteractionManager::SetInteracting(const bool bInteract)
{
	const bool bShouldInteract = bInteract && IsActive();
	if (bWantInteract != bShouldInteract)
	{
		bWantInteract = bShouldInteract;
		if (Target.IsValid())
		{
			if (bWantInteract && !bActiveInteract)
			{
				bActiveInteract = true;
				IInteractable::BeginInteract(Target.Get(), GetOwner<AToroCharacter>());
			}
			else if (!bWantInteract && bActiveInteract)
			{
				bActiveInteract = false;
				IInteractable::EndInteract(Target.Get(), GetOwner<AToroCharacter>());
			}
		}
	}
}

void UInteractionManager::Deactivate()
{
	SetTarget(nullptr);
	bWantInteract = false;
	Super::Deactivate();
}

void UInteractionManager::SetTarget(AActor* InTarget)
{
	if (Target != InTarget)
	{
		if (bActiveInteract && Target.IsValid())
		{
			bActiveInteract = false;
			IInteractable::EndInteract(Target.Get(), GetOwner<AToroCharacter>());
		}

		Target = InTarget;
		OnTargetChanged.Broadcast(Target);

		if (bWantInteract && Target.IsValid())
		{
			bActiveInteract = true;
			IInteractable::BeginInteract(Target.Get(), GetOwner<AToroCharacter>());
		}
	}
}

void UInteractionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (!ensureAlwaysMsgf(ExecuteTraceLogic.IsBound(), TEXT("ExecuteTraceLogic is not bound on %s"), *GetName()))
	{
		Deactivate();
	}

	if (AActor* HitActor = ExecuteTraceLogic.Execute())
	{
		FText Label;
		SetTarget(IInteractable::GetInteractionInfo(HitActor, Label) ? HitActor : nullptr);
	}
	else
	{
		SetTarget(nullptr);
	}
}
