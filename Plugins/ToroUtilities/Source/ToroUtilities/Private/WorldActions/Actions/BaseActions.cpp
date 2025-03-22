// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/BaseActions.h"
#include "EnhancedCodeFlow.h"

void FWControlDelay::RunEvent(const UObject* WorldContext)
{
	if (FFlow::IsActionRunning(WorldContext, DelayHandle) && !bRetriggerable) return;

	FFlow::StopAction(WorldContext, DelayHandle);
	DelayHandle = FFlow::Delay(WorldContext, Delay, [this, WorldContext]()
	{
		FOR_EACH_ACTION(Actions, {
			ActionPtr->RunEvent(WorldContext);
		})
	});
}

void FWControlDelay::OnBeginPlay(const UObject* WorldContext)
{
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnBeginPlay(WorldContext);
	})
}

void FWControlDelay::OnTick(const UObject* WorldContext, const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(WorldContext, DeltaTime);
	})
}

void FWControlDelay::OnPostEditChange(const UObject* WorldContext)
{
	bCanEverTick = false;
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnPostEditChange(WorldContext);
		if (!bCanEverTick) bCanEverTick = ActionPtr->bCanEverTick;
	})
}

void FWControlFlipFlop::RunEvent(const UObject* WorldContext)
{
	bIsA = !bIsA;
	TArray<TInstancedStruct<FWorldActionBase>>& Events = bIsA ? Flip : Flop;
	FOR_EACH_ACTION(Events, {
		ActionPtr->RunEvent(WorldContext);
	})
}

void FWControlFlipFlop::OnBeginPlay(const UObject* WorldContext)
{
	FOR_EACH_ACTION(Flip, {
		ActionPtr->OnBeginPlay(WorldContext);
	})
	FOR_EACH_ACTION(Flop, {
		ActionPtr->OnBeginPlay(WorldContext);
	})
}

void FWControlFlipFlop::OnTick(const UObject* WorldContext, const float DeltaTime)
{
	FOR_EACH_ACTION(Flip, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(WorldContext, DeltaTime);
	})
	FOR_EACH_ACTION(Flop, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(WorldContext, DeltaTime);
	})
}

void FWControlFlipFlop::OnPostEditChange(const UObject* WorldContext)
{
	bCanEverTick = false;
	FOR_EACH_ACTION(Flip, {
		ActionPtr->OnPostEditChange(WorldContext);
		if (!bCanEverTick) bCanEverTick = ActionPtr->bCanEverTick;
	})
	FOR_EACH_ACTION(Flop, {
		ActionPtr->OnPostEditChange(WorldContext);
		if (!bCanEverTick) bCanEverTick = ActionPtr->bCanEverTick;
	})
}

void FWControlDoOnce::RunEvent(const UObject* WorldContext)
{
	if (!bHasDone)
	{
		bHasDone = true;
		FOR_EACH_ACTION(Actions, {
			ActionPtr->RunEvent(WorldContext);
		})
	}
}

void FWControlDoOnce::OnBeginPlay(const UObject* WorldContext)
{
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnBeginPlay(WorldContext);
	})
}

void FWControlDoOnce::OnTick(const UObject* WorldContext, const float DeltaTime)
{
	FOR_EACH_ACTION(Actions, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(WorldContext, DeltaTime);
	})
}

void FWControlDoOnce::OnPostEditChange(const UObject* WorldContext)
{
	bCanEverTick = false;
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnPostEditChange(WorldContext);
		if (!bCanEverTick) bCanEverTick = ActionPtr->bCanEverTick;
	})
}
