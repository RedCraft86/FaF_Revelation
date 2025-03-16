// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroAsyncActionBase.h"
#include "LoggingHelpers.h"

UWorld* UToroAsyncActionBase::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}

bool UToroAsyncActionBase::IsRunningChecked()
{
	if (bTaskRunning)
	{
		UE_KLOG_WARNING(2.0f, TEXT("Async Action %s is already running. Ignoring new call."), *GetClass()->GetName())
		return true;
	}

	bTaskRunning = true;
	return false;
}
