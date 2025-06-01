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
	FOR_EACH_ACTION_PTR(ActionPtrs, this, ActionPtr->RunAction();)
}

void UWorldActionComponent::UpdateEdFunctions()
{
	bool bWantsTick = false;
	FOR_EACH_ACTION_PTR(ActionPtrs, this,
		ActionPtr->OnPostEditChange();
		if (!bWantsTick) bWantsTick = ActionPtr->bShouldTick;
	)
	PrimaryComponentTick.bStartWithTickEnabled = bWantsTick;
}

void UWorldActionComponent::BeginPlay()
{
	Super::BeginPlay();
	FOR_EACH_ACTION_PTR(ActionPtrs, this, ActionPtr->OnBeginPlay();)
}

void UWorldActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	FOR_EACH_ACTION_PTR(ActionPtrs, this, if (ActionPtr->bShouldTick) ActionPtr->OnTick(DeltaTime);)
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