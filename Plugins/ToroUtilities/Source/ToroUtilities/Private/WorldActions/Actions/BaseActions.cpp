// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/BaseActions.h"
#include "EnhancedCodeFlow.h"

bool FWAFlowDelay::ShouldTick() const
{
	if (!Super::ShouldTick()) return false;
	FOR_EACH_ACTION_CONST(Actions, if (Ptr->ShouldTick()) return true;)
	return false;
}

void FWAFlowDelay::RunAction()
{
	if (!FFlow::IsActionRunning(GetWorldContext(), DelayHandle) || bRetriggerable)
	{
		FFlow::StopAction(GetWorldContext(), DelayHandle);
		DelayHandle = FFlow::Delay(GetWorldContext(), Delay, [this]()
		{
			FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->RunAction();)
		});
	}
}

void FWAFlowDelay::OnBeginPlay()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->OnBeginPlay();)
}

void FWAFlowDelay::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), if (Ptr->ShouldTick()) Ptr->OnTick(DeltaTime);)
}

void FWAFlowDelay::OnConstruction()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->OnConstruction();)
}

bool FWAFlowDoOnce::ShouldTick() const
{
	if (!Super::ShouldTick()) return false;
	FOR_EACH_ACTION_CONST(Actions, if (Ptr->ShouldTick()) return true;)
	return false;
}

void FWAFlowDoOnce::RunAction()
{
	if (bHasRan)
	{
		bHasRan = true;
		FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->RunAction();)
	}
}

void FWAFlowDoOnce::OnBeginPlay()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->OnBeginPlay();)
}

void FWAFlowDoOnce::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), if (Ptr->ShouldTick()) Ptr->OnTick(DeltaTime);)
}

void FWAFlowDoOnce::OnConstruction()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->OnConstruction();)
}

bool FWAFlowFlipFlop::ShouldTick() const
{
	if (!Super::ShouldTick()) return false;
	FOR_EACH_ACTION_CONST(Flip, if (Ptr->ShouldTick()) return true;)
	FOR_EACH_ACTION_CONST(Flop, if (Ptr->ShouldTick()) return true;)
	return false;
}

void FWAFlowFlipFlop::RunAction()
{
	bIsA = !bIsA;
	TArray<TInstancedStruct<FWorldActionBase>>& Actions = bIsA ? Flip : Flop;
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->RunAction();)
}

void FWAFlowFlipFlop::OnBeginPlay()
{
	FOR_EACH_ACTION(Flip, GetWorldContext(), Ptr->OnBeginPlay();)
	FOR_EACH_ACTION(Flop, GetWorldContext(), Ptr->OnBeginPlay();)
}

void FWAFlowFlipFlop::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Flip, GetWorldContext(), if (Ptr->ShouldTick()) Ptr->OnTick(DeltaTime);)
	FOR_EACH_ACTION(Flop, GetWorldContext(), if (Ptr->ShouldTick()) Ptr->OnTick(DeltaTime);)
}

void FWAFlowFlipFlop::OnConstruction()
{
	FOR_EACH_ACTION(Flip, GetWorldContext(), Ptr->OnConstruction();)
	FOR_EACH_ACTION(Flop, GetWorldContext(), Ptr->OnConstruction();)
}
