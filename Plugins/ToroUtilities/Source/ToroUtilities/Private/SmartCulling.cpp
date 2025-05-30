// Copyright (C) RedCraft86. All Rights Reserved.

#include "SmartCulling.h"

USmartCullingComponent::USmartCullingComponent()
	: bAffectTicking(false), bDisabled(false), bCachedHiddenState(false), bInitialTickState(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;
#if WITH_EDITOR
	bTickInEditor = false;
#endif
}

USmartCullingComponent* USmartCullingComponent::Get(const AActor* Target)
{
	TArray<USmartCullingComponent*> Comps;
	if (Target) Target->GetComponents<USmartCullingComponent>(Comps);
	if (Comps.Num() == 1) return Comps[0];
	return nullptr;
}

void USmartCullingComponent::AddRenderRequest(const UObject* Object)
{
	if (bDisabled || !Object || RenderRequests.Contains(Object)) return;
	RenderRequests.Add(Object);
	UpdateRenderingState();
}

void USmartCullingComponent::RemoveRenderRequest(const UObject* Object)
{
	if (bDisabled || !Object) return;
	if (RenderRequests.Remove(Object) > 0)
	{
		UpdateRenderingState();
	}
}

void USmartCullingComponent::CheckRenderRequests()
{
	bool bAdjusted = false;
	for (auto It = RenderRequests.CreateIterator(); It; ++It)
	{
		if (It->IsNull() || !It->LoadSynchronous())
		{
			It.RemoveCurrent();
			bAdjusted = true;
		}
	}

	if (bAdjusted)
		UpdateRenderingState();
}

void USmartCullingComponent::UpdateRenderingState()
{
	if (bDisabled) return;
	if (bCachedHiddenState != RenderRequests.IsEmpty())
	{
		bCachedHiddenState = RenderRequests.IsEmpty();
		GetOwner()->SetActorHiddenInGame(bCachedHiddenState);
		if (bAffectTicking)
		{
			if (bCachedHiddenState)
			{
				bInitialTickState = GetOwner()->IsActorTickEnabled();
				GetOwner()->SetActorTickEnabled(false);
			}
			else
			{
				GetOwner()->SetActorTickEnabled(bInitialTickState);
			}
		}
	}
}

void USmartCullingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (bDisabled)
	{
		DestroyComponent();
		return;
	}

	TArray<USmartCullingComponent*> Comps;
	GetOwner()->GetComponents<USmartCullingComponent>(Comps);
#if WITH_EDITOR
	ensureMsgf(Comps.Num() == 1, TEXT("%d Smart Culling Components found on actor %s (%s) [%s]."), Comps.Num(),
		*GetOwner()->GetActorLabel(), *GetOwner()->GetClass()->GetName(), *TSoftObjectPtr(GetOwner()).ToString());
#endif
	if (Comps.Num() != 1)
	{
		bDisabled = true;
		DestroyComponent();
		return;
	}
	
	UpdateRenderingState();
}

void USmartCullingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckRenderRequests();
}