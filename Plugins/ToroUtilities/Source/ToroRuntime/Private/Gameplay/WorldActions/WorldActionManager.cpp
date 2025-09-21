// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/WorldActionManager.h"

UWorldActionManager::UWorldActionManager(): bAutoConstruction(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UWorldActionManager::SetActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions)
{
	Actions = MoveTemp(InActions);
	RunConstruction();
}

void UWorldActionManager::AppendActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions)
{
	Actions.Append(MoveTemp(InActions));
	RunConstruction();
}

void UWorldActionManager::RunActions()
{
	ForEachAction([](FWorldActionBase* Ptr)
	{
		Ptr->RunAction();
	});
}

void UWorldActionManager::RunConstruction()
{
	bool bWantsTick = false;
	ForEachAction([&bWantsTick](FWorldActionBase* Ptr)
	{
		Ptr->OnConstruction();
		bWantsTick |= Ptr->ShouldTick();
	});
	PrimaryComponentTick.bStartWithTickEnabled = bWantsTick;
}

void UWorldActionManager::ForEachAction(const TFunction<void(FWorldActionBase*)>& Func)
{
	for (auto It = Actions.CreateIterator(); It; ++It)
	{
		TInstancedStruct<FWorldActionBase>& Action = *It;
		if (FWorldActionBase* Ptr = Action.GetMutablePtr())
		{
			Ptr->SetWorldContext(this);
			if (Func) Func(Ptr);
		}
		else
		{
			It.RemoveCurrentSwap();
		}
	}
}

void UWorldActionManager::BeginPlay()
{
	Super::BeginPlay();
	ForEachAction([](FWorldActionBase* Ptr)
	{
		Ptr->OnBeginPlay();
	});
}

void UWorldActionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	ForEachAction([DeltaTime](FWorldActionBase* Ptr)
	{
		Ptr->OnTick(DeltaTime);
	});
}

#if WITH_EDITOR
void UWorldActionManager::PostLoad()
{
	Super::PostLoad();
	if (bAutoConstruction) RunConstruction();
}

void UWorldActionManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (bAutoConstruction) RunConstruction();
}
#endif