// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/ReferenceCulling.h"

URefCullComponent::URefCullComponent(): bAffectTicking(false), bCachedTickState(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 2.0f;
}

URefCullComponent* URefCullComponent::Get(const AActor* Target)
{
	return Target ? Target->FindComponentByClass<URefCullComponent>() : nullptr;
}

void URefCullComponent::AddRenderRequest(const UObject* Object)
{
	if (Object && !Requests.Contains(Object))
	{
		Requests.Add(Object);
		UpdateRenderingState();
	}
}

void URefCullComponent::RemoveRenderRequest(const UObject* Object)
{
	if (Object && Requests.Remove(Object) > 0)
	{
		UpdateRenderingState();
	}
}

void URefCullComponent::UpdateRenderingState()
{
	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->SetActorHiddenInGame(Requests.IsEmpty());
		if (bAffectTicking)
		{
			if (OwnerActor->IsHidden())
			{
				bCachedTickState = OwnerActor->IsActorTickEnabled();
				OwnerActor->SetActorTickEnabled(false);
			}
			else
			{
				OwnerActor->SetActorTickEnabled(bCachedTickState);
			}
		}
	}
}

void URefCullComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	bool bChanged = false;
	for (auto It = Requests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			bChanged = true;
		}
	}
	if (bChanged) UpdateRenderingState();
}
