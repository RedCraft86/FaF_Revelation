// Copyright (C) RedCraft86. All Rights Reserved.

#include "MiscClasses/RefCullingComponent.h"

URefCullingComponent::URefCullingComponent(): bAffectTicking(false), bDisabled(false)
	, bCachedHiddenState(false), bInitialTickState(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;
#if WITH_EDITOR
	bTickInEditor = false;
#endif
}

URefCullingComponent* URefCullingComponent::Get(const AActor* Target)
{
	TArray<URefCullingComponent*> Comps;
	if (Target) Target->GetComponents<URefCullingComponent>(Comps);
	if (Comps.Num() == 1) return Comps[0];
	return nullptr;
}

void URefCullingComponent::AddRenderRequest(const UObject* Object)
{
	if (bDisabled || !Object || RenderRequests.Contains(Object)) return;
	RenderRequests.Add(Object);
	UpdateRenderingState();
}

void URefCullingComponent::RemoveRenderRequest(const UObject* Object)
{
	if (bDisabled || !Object) return;
	if (RenderRequests.Remove(Object) > 0)
	{
		UpdateRenderingState();
	}
}

void URefCullingComponent::CheckRenderRequests()
{
	int32 NumRemoved = 0;
	for (auto It = RenderRequests.CreateIterator(); It; ++It)
	{
		if (It->IsNull() || !IsValid(It->LoadSynchronous()))
		{
			It.RemoveCurrent();
			NumRemoved++;
		}
	}

	if (NumRemoved > 0)
	{
		UpdateRenderingState();
	}
}

void URefCullingComponent::UpdateRenderingState()
{
	if (bDisabled) return;
	const bool bShouldHide = RenderRequests.IsEmpty();
	if (bCachedHiddenState != bShouldHide)
	{
		bCachedHiddenState = bShouldHide;
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

void URefCullingComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<URefCullingComponent*> Comps;
	GetOwner()->GetComponents<URefCullingComponent>(Comps);
#if WITH_EDITOR
	ensureMsgf(Comps.Num() == 1, TEXT("%d Reference Culling Components found on actor %s (%s) [%s]."), Comps.Num(),
		*TSoftObjectPtr(GetOwner()).ToString(), *GetOwner()->GetActorLabel(), *GetOwner()->GetClass()->GetName());
#endif
	if (Comps.Num() != 1)
	{
		bDisabled = true;
		DestroyComponent();
		return;
	}
	
	UpdateRenderingState();
}

void URefCullingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	CheckRenderRequests();
}
