// Copyright (C) RedCraft86. All Rights Reserved.

#include "ZoneCullingComponent.h"

UZoneCullingComponent::UZoneCullingComponent() : bDisableComponent(false)
	, bAffectTicking(false), bCachedHiddenState(false), bInitialTickState(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.0f;
#if WITH_EDITOR
	bTickInEditor = false;
#endif
}

EToroFoundPins UZoneCullingComponent::GetZoneCullingComponent(UZoneCullingComponent*& OutComponent, const AActor* Target)
{
	OutComponent = GetZoneCullingComponent(Target);
	return OutComponent ? EToroFoundPins::Found : EToroFoundPins::NotFound;
}

UZoneCullingComponent* UZoneCullingComponent::GetZoneCullingComponent(const AActor* Target)
{
	TArray<UZoneCullingComponent*> Comps;
	if (Target) Target->GetComponents<UZoneCullingComponent>(Comps);
	if (Comps.Num() == 1) return Comps[0];
	return nullptr;
}

void UZoneCullingComponent::AddRenderRequest(const UObject* Object)
{
	if (bDisableComponent || !Object || RenderRequests.Contains(Object)) return;
	RenderRequests.Add(Object);
	UpdateRenderingState();
}

void UZoneCullingComponent::RemoveRenderRequest(const UObject* Object)
{
	if (bDisableComponent || !Object) return;
	if (RenderRequests.Remove(Object) > 0)
	{
		UpdateRenderingState();
	}
}

void UZoneCullingComponent::CheckRenderRequests()
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

void UZoneCullingComponent::UpdateRenderingState()
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

void UZoneCullingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (bDisableComponent) DestroyComponent();

	TArray<UZoneCullingComponent*> Comps;
	GetOwner()->GetComponents<UZoneCullingComponent>(Comps);
#if WITH_EDITOR
	ensureMsgf(Comps.Num() == 1, TEXT("Multiple Smart Culling Components found on actor %s (%s) [%s]."),
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

void UZoneCullingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	CheckRenderRequests();
}
