// Copyright (C) RedCraft86. All Rights Reserved.

#include "SmartCullingComponent.h"

USmartCullingComponent::USmartCullingComponent() : bAffectTicking(false)
	, bDisableComponent(false), bCachedHiddenState(false), bInitialTickState(false)
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
	if (bDisableComponent || !Object || RenderRequests.Contains(Object)) return;
	RenderRequests.Add(Object);
	UpdateRenderingState();
}

void USmartCullingComponent::RemoveRenderRequest(const UObject* Object)
{
	if (bDisableComponent || !Object) return;
	if (RenderRequests.Remove(Object) > 0)
	{
		UpdateRenderingState();
	}
}

void USmartCullingComponent::CheckRenderRequests()
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

void USmartCullingComponent::UpdateRenderingState()
{
	if (bDisableComponent) return;
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
	if (bDisableComponent)
	{
		DestroyComponent();
		return;
	}

	TArray<USmartCullingComponent*> Comps;
	GetOwner()->GetComponents<USmartCullingComponent>(Comps);
#if WITH_EDITOR
	ensureMsgf(Comps.Num() == 1, TEXT("%d Smart Culling Components found on actor %s (%s) [%s]."), Comps.Num(),
		*TSoftObjectPtr(GetOwner()).ToString(), *GetOwner()->GetActorLabel(), *GetOwner()->GetClass()->GetName());
#endif
	if (Comps.Num() != 1)
	{
		bDisableComponent = true;
		DestroyComponent();
		return;
	}
	
	UpdateRenderingState();
}

void USmartCullingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	CheckRenderRequests();
}
