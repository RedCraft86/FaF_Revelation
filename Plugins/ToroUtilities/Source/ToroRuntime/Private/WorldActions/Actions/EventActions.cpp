// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/EventActions.h"
#include "Engine/LevelScriptActor.h"
#include "Misc/OutputDeviceNull.h"

void FWTaskEventRemote::RunEvent(const UObject* WorldContext)
{
	if (EventName.IsNone()) return;
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
	{
		LSA->RemoteEvent(EventName);
	}
}

void FWTaskEventActor::RunEvent(const UObject* WorldContext)
{
	if (EventName.IsNone()) return;

	FOutputDeviceNull Ar;
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		Ptr->CallFunctionByNameWithArguments(*EventName.ToString(),
			Ar, nullptr, true);
	})
}
