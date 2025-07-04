// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/ReferenceCulling.h"
#include "ToroCore.h"

URefCullComponent::URefCullComponent(): bAffectTicking(false), bEnabled(true), bCachedTick(false)
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

void URefCullComponent::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		UpdateRenderingState();
	}
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
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->SetActorHiddenInGame(bEnabled && Requests.IsEmpty());
		if (bAffectTicking)
		{
			if (OwnerActor->IsHidden())
			{
				bCachedTick = OwnerActor->IsActorTickEnabled();
				OwnerActor->SetActorTickEnabled(false);
			}
			else
			{
				OwnerActor->SetActorTickEnabled(*bCachedTick);
				bCachedTick.Reset();
			}
		}
	}
}

void URefCullComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<URefCullComponent*> Components;
	GetOwner()->GetComponents<URefCullComponent>(Components);
	if (Components.Num() > 1 && Components[0] != this)
	{
		UE_LOG(LogToroCore, Warning, TEXT(
			"Only one instance of Reference Culling can exist per actor! Removing %s"), *GetName())

		SetComponentTickEnabled(false);
		GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr(this)]()
		{
			if (WeakThis.IsValid()) WeakThis->DestroyComponent();
		});
	}
}

void URefCullComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (!bEnabled) return;
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
