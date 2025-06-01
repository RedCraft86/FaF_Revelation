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
		FOR_EACH_ACTION(Actions, GetWorldContext(), ActionPtr->RunAction();)
	});
}

void FWAFlowDelay::OnBeginPlay()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), ActionPtr->OnBeginPlay();)
}

void FWAFlowDelay::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), if (ActionPtr->bShouldTick) ActionPtr->OnTick(DeltaTime);)
}

void FWAFlowDelay::OnPostEditChange()
{
	bShouldTick = false;
	FOR_EACH_ACTION(Actions, GetWorldContext(),
		ActionPtr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = ActionPtr->bShouldTick;
	)
}

void FWAFlowDoOnce::RunAction()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), ActionPtr->RunAction();)
}

void FWAFlowDoOnce::OnBeginPlay()
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), ActionPtr->OnBeginPlay();)
}

void FWAFlowDoOnce::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, GetWorldContext(), if (ActionPtr->bShouldTick) ActionPtr->OnTick(DeltaTime);)
}

void FWAFlowDoOnce::OnPostEditChange()
{
	bShouldTick = false;
	FOR_EACH_ACTION(Actions, GetWorldContext(),
		ActionPtr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = ActionPtr->bShouldTick;
	)
}

void FWAFlowFlipFlop::RunAction()
{
	bIsA = !bIsA;
	TArray<TInstancedStruct<FWorldActionBase>>& Actions = bIsA ? Flip : Flop;
	FOR_EACH_ACTION(Actions, GetWorldContext(), ActionPtr->RunAction();)
}

void FWAFlowFlipFlop::OnBeginPlay()
{
	FOR_EACH_ACTION(Flip, GetWorldContext(), ActionPtr->OnBeginPlay();)
	FOR_EACH_ACTION(Flop, GetWorldContext(), ActionPtr->OnBeginPlay();)
}

void FWAFlowFlipFlop::OnTick(const float DeltaTime)
{
	FOR_EACH_ACTION(Flip, GetWorldContext(), if (ActionPtr->bShouldTick) ActionPtr->OnTick(DeltaTime);)
	FOR_EACH_ACTION(Flop, GetWorldContext(), if (ActionPtr->bShouldTick) ActionPtr->OnTick(DeltaTime);)
}

void FWAFlowFlipFlop::OnPostEditChange()
{
	bShouldTick = false;

	FOR_EACH_ACTION(Flip, GetWorldContext(),
		ActionPtr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = ActionPtr->bShouldTick;
	)

	FOR_EACH_ACTION(Flop, GetWorldContext(),
		ActionPtr->OnPostEditChange();
		if (!bShouldTick) bShouldTick = ActionPtr->bShouldTick;
	)
}
