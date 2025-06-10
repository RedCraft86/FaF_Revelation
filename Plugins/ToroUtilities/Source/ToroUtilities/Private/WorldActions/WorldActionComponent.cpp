// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/WorldActionComponent.h"

UWorldActionComponent::UWorldActionComponent(): bAutoConstruction(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UWorldActionComponent::SetActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions, const bool bConstruct)
{
	ActionPtrs.Empty(InActions.Num());
	for (TInstancedStruct<FWorldActionBase>& Action : InActions)
	{
		ActionPtrs.Add(Action.GetMutablePtr());
	}
	if (bConstruct) RunConstruction();
}

void UWorldActionComponent::AppendActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions, const bool bConstruct)
{
	ActionPtrs.Reserve(ActionPtrs.Num() + InActions.Num());
	for (TInstancedStruct<FWorldActionBase>& Action : InActions)
	{
		ActionPtrs.Add(Action.GetMutablePtr());
	}
	if (bConstruct) RunConstruction();
}

void UWorldActionComponent::RunActions()
{
	FOR_EACH_ACTION_PTR(ActionPtrs, this,
		Ptr->RunAction();
	)
}

void UWorldActionComponent::RunConstruction()
{
	bool bWantsTick = false;
	FOR_EACH_ACTION_PTR(ActionPtrs, this,
		if (!FApp::IsGame()) Ptr->OnConstruction();
		if (!bWantsTick) bWantsTick = Ptr->ShouldTick();
	)
	PrimaryComponentTick.bStartWithTickEnabled = bWantsTick;
}

void UWorldActionComponent::BeginPlay()
{
	Super::BeginPlay();
	FOR_EACH_ACTION_PTR(ActionPtrs, this,
		Ptr->OnBeginPlay();
	)
}

void UWorldActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	FOR_EACH_ACTION_PTR(ActionPtrs, this,
		if (Ptr->ShouldTick()) Ptr->OnTick(DeltaTime);
	)
}

#if WITH_EDITOR
void UWorldActionComponent::PostLoad()
{
	Super::PostLoad();
	if (bAutoConstruction) RunConstruction();
}

void UWorldActionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (bAutoConstruction) RunConstruction();
}
#endif
