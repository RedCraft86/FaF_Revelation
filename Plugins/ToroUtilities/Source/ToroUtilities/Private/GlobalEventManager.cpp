// Copyright (C) RedCraft86. All Rights Reserved.

#include "GlobalEvents/GlobalEventManager.h"

DEFINE_GAMEPLAY_TAG_BASE(GlobalEvent)

void UGlobalEventManager::BindGlobalEvent(const FGameplayTag EventID, const FGlobalEventSingle& Event)
{
	if (!VerifyGlobalEventTag(EventID) || !Event.IsBound()) return;
	Events.FindOrAdd(EventID).Add(Event);
}

void UGlobalEventManager::UnbindGlobalEvent(const FGameplayTag EventID, const FGlobalEventSingle& Event)
{
	if (!VerifyGlobalEventTag(EventID) || !Event.IsBound()) return;
	if (FGlobalEventMulti* MultiEvent = Events.Find(EventID))
	{
		MultiEvent->Remove(Event);
		if (!MultiEvent->IsBound())
		{
			RemoveGlobalEvent(EventID);
		}
	}
}

void UGlobalEventManager::InvokeGlobalEvent(const FGameplayTag EventID, UGlobalEventPayload* Payload, UObject* Instigator)
{
	if (!VerifyGlobalEventTag(EventID)) return;
	if (!Events.Contains(EventID)) return;
	if (Payload)
	{
		Payload->EventInstigator = Instigator;
		Payloads.Add(EventID, Payload);
	}
	else Payloads.Remove(EventID);
	Events.Find(EventID)->Broadcast(Payload);
}

void UGlobalEventManager::RemoveGlobalEvent(const FGameplayTag EventID)
{
	if (!VerifyGlobalEventTag(EventID)) return;
	if (FGlobalEventMulti* MultiEvent = Events.Find(EventID))
	{
		MultiEvent->Clear();
	}
	Events.Remove(EventID);
	Payloads.Remove(EventID);
}
