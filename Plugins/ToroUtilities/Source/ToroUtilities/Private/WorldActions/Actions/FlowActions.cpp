// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/FlowActions.h"
#include "EnhancedCodeFlow.h"

void FWAFlowDelay::RunAction()
{
	if (FFlow::IsActionRunning(GetWorldContext(), DelayHandle)
		&& !bRetriggerable)
		return;

	FFlow::StopAction(GetWorldContext(), DelayHandle);
	DelayHandle = FFlow::Delay(GetWorldContext(), Delay, [this]()
	{
		FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->RunAction();)
	});
}

void FWAFlowDelay::OnBeginPlay()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->OnBeginPlay();)
}

void FWAFlowDelay::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), if (Ptr->bShouldTick) Ptr->OnTick(DeltaTime);)
}

void FWAFlowDelay::OnPostEditChange()
{
	bShouldTick = false;
	FOR_EACH_ACTION(Actions, GetWorldContext(),
		Ptr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = Ptr->bShouldTick;
	)
}

void FWAFlowDoOnce::RunAction()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->RunAction();)
}

void FWAFlowDoOnce::OnBeginPlay()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), Ptr->OnBeginPlay();)
}

void FWAFlowDoOnce::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), if (Ptr->bShouldTick) Ptr->OnTick(DeltaTime);)
}

void FWAFlowDoOnce::OnPostEditChange()
{
	bShouldTick = false;
	FOR_EACH_ACTION(Actions, GetWorldContext(),
		Ptr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = Ptr->bShouldTick;
	)
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
	FOR_EACH_ACTION(Flip, GetWorldContext(), if (Ptr->bShouldTick) Ptr->OnTick(DeltaTime);)
	FOR_EACH_ACTION(Flop, GetWorldContext(), if (Ptr->bShouldTick) Ptr->OnTick(DeltaTime);)
}

void FWAFlowFlipFlop::OnPostEditChange()
{
	bShouldTick = false;

	FOR_EACH_ACTION(Flip, GetWorldContext(),
		Ptr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = Ptr->bShouldTick;
	)

	FOR_EACH_ACTION(Flop, GetWorldContext(),
		Ptr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = Ptr->bShouldTick;
	)
}
