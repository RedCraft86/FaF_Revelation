// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/WorldActionComponent.h"

UWorldActionComponent::UWorldActionComponent()
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

void UWorldActionComponent::RunActions()
{
	ForEachAction([](FWorldActionBase* Action)
	{
		Action->RunAction();
	});
}

void UWorldActionComponent::ForEachAction(const TFunction<void(FWorldActionBase*)>& Func)
{
	for (FWorldActionBase* ActionPtr : ActionPtrs)
	{
		if (ActionPtr)
		{
			ActionPtr->SetWorldContext(this);
			if (Func) Func(ActionPtr);
		}
	}
}

void UWorldActionComponent::BeginPlay()
{
	Super::BeginPlay();
	ForEachAction([](FWorldActionBase* Action)
	{
		Action->OnBeginPlay();
	});
}

void UWorldActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	ForEachAction([DeltaTime](FWorldActionBase* Action)
	{
		if (Action->bShouldTick) Action->OnTick(DeltaTime);
	});
}

#if WITH_EDITOR
void UWorldActionComponent::PostLoad()
{
	Super::PostLoad();
	if (!NativeActions.IsEmpty())
	{
		SetActions(NativeActions);
	}

	bool bWantsTick = false;
	ForEachAction([&bWantsTick](FWorldActionBase* Action)
	{
		Action->OnPostEditChange();
		if (!bWantsTick) bWantsTick = Action->bShouldTick;
	});
	PrimaryComponentTick.bStartWithTickEnabled = bWantsTick;
}

void UWorldActionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!NativeActions.IsEmpty())
	{
		SetActions(NativeActions);
	}

	bool bWantsTick = false;
	ForEachAction([&bWantsTick](FWorldActionBase* Action)
	{
		Action->OnPostEditChange();
		if (!bWantsTick) bWantsTick = Action->bShouldTick;
	});
	PrimaryComponentTick.bStartWithTickEnabled = bWantsTick;
}
#endif