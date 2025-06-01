// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/EventActions.h"
#include "GlobalEvents/GlobalEventManager.h"
#include "Engine/LevelScriptActor.h"
#include "Misc/OutputDeviceNull.h"

void FWAEventRemote::RunAction()
{
	if (EventName.IsNone()) return;
	const UWorld* World = GEngine->GetWorldFromContextObject(GetWorldContext(), EGetWorldErrorMode::LogAndReturnNull);
	if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
	{
		LSA->RemoteEvent(EventName);
	}
}

void FWAEventActor::RunAction()
{
	if (EventName.IsNone()) return;

	FOutputDeviceNull Ar;
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		Ptr->CallFunctionByNameWithArguments(*EventName.ToString(),
			Ar, nullptr, true);
	})
}

void FWAEventGlobal::RunAction()
{
	if (UGlobalEventManager* Manager = UGlobalEventManager::Get(GetWorldContext()))
	{
		UObject* Instigator = nullptr;
		UGlobalEventPayload* Payload = nullptr;
		if (bIncludePayload)
		{
			Instigator = OptionalInstigator.LoadSynchronous();
			if (!Instigator) Instigator = const_cast<UObject*>(GetWorldContext());

			Payload = NewObject<UGlobalEventPayload>(Instigator);
			if (Payload)
			{
				Payload->Strings = PayloadStrings;
				Payload->Floats = PayloadFloats;
			}
		}
		Manager->InvokeGlobalEvent(EventID, Payload, Instigator);
	}
}
