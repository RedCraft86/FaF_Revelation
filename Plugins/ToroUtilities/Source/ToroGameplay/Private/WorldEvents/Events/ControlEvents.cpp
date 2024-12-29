// Copyright (C) RedCraft86. All Rights Reserved.


#include "WorldEvents/Events/ControlEvents.h"

void FWCDelay::RunEvent(const UObject* WorldContext)
{
	FWCBase::RunEvent(WorldContext);
}

void FWCDelay::OnConstruction(const UObject* WorldContext, const bool bEditorTime)
{
	for (TInstancedStruct<FWorldEventBase>& Event : Events)
	{
		if (FWorldEventBase* EventPtr = Event.GetMutablePtr<FWorldEventBase>())
		{
			EventPtr->OnConstruction(WorldContext, bEditorTime);
		}
	}
}

void FWCFlipFlop::RunEvent(const UObject* WorldContext)
{
	bIsA = !bIsA;
	TArray<TInstancedStruct<FWorldEventBase>>& Events = bIsA ? Flip : Flop;
	for (TInstancedStruct<FWorldEventBase>& Event : Events)
	{
		if (FWorldEventBase* EventPtr = Event.GetMutablePtr<FWorldEventBase>())
		{
			EventPtr->RunEvent(WorldContext);
		}
	}
}

void FWCFlipFlop::OnConstruction(const UObject* WorldContext, const bool bEditorTime)
{
	for (TInstancedStruct<FWorldEventBase>& Event : Flip)
	{
		if (FWorldEventBase* EventPtr = Event.GetMutablePtr<FWorldEventBase>())
		{
			EventPtr->OnConstruction(WorldContext, bEditorTime);
		}
	}

	for (TInstancedStruct<FWorldEventBase>& Event : Flop)
	{
		if (FWorldEventBase* EventPtr = Event.GetMutablePtr<FWorldEventBase>())
		{
			EventPtr->OnConstruction(WorldContext, bEditorTime);
		}
	}
}

void FWCDoOnce::RunEvent(const UObject* WorldContext)
{
	if (bHasDone) return;
	
	bHasDone = true;
	for (TInstancedStruct<FWorldEventBase>& Event : Events)
	{
		if (FWorldEventBase* EventPtr = Event.GetMutablePtr<FWorldEventBase>())
		{
			EventPtr->RunEvent(WorldContext);
		}
	}
}

void FWCDoOnce::OnConstruction(const UObject* WorldContext, const bool bEditorTime)
{
	for (TInstancedStruct<FWorldEventBase>& Event : Events)
	{
		if (FWorldEventBase* EventPtr = Event.GetMutablePtr())
		{
			EventPtr->OnConstruction(WorldContext, bEditorTime);
		}
	}
}
