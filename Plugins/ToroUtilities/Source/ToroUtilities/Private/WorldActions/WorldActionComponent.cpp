// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/WorldActionComponent.h"

UWorldActionComponent::UWorldActionComponent(): bManualEdFunction(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UWorldActionComponent::SetActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions)
{
	ActionPtrs.Empty(InActions.Num());
	for (TInstancedStruct<FWorldActionBase>& Action : InActions)
	{
		ActionPtrs.Add(Action.GetMutablePtr());
	}
}

void UWorldActionComponent::AppendActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions)
{
	ActionPtrs.Reserve(ActionPtrs.Num() + InActions.Num());
	for (TInstancedStruct<FWorldActionBase>& Action : InActions)
	{
		ActionPtrs.Add(Action.GetMutablePtr());
	}
}

void UWorldActionComponent::RunActions()
{
	FOR_EACH_ACTION_PTR(ActionPtrs, this, Ptr->RunAction();)
}

void UWorldActionComponent::UpdateEdFunctions()
{
	bool bWantsTick = false;
	FOR_EACH_ACTION_PTR(ActionPtrs, this,
		if (!FApp::IsGame()) Ptr->OnPostEditChange();
		if (!bWantsTick) bWantsTick = Ptr->bShouldTick;
	)
	PrimaryComponentTick.bStartWithTickEnabled = bWantsTick;
}

void UWorldActionComponent::BeginPlay()
{
	Super::BeginPlay();
	FOR_EACH_ACTION_PTR(ActionPtrs, this, Ptr->OnBeginPlay();)
}

void UWorldActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	FOR_EACH_ACTION_PTR(ActionPtrs, this, if (Ptr->bShouldTick) Ptr->OnTick(DeltaTime);)
}

#if WITH_EDITOR
void UWorldActionComponent::PostLoad()
{
	Super::PostLoad();
	if (!bManualEdFunction) UpdateEdFunctions();
}

void UWorldActionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!bManualEdFunction) UpdateEdFunctions();
}
#endif