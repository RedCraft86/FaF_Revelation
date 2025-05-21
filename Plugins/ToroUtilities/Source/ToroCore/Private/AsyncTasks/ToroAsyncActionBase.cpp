// Copyright (C) RedCraft86. All Rights Reserved.

#include "AsyncTasks/ToroAsyncActionBase.h"
#include "Helpers/LoggingMacros.h"

UWorld* UToroAsyncActionBase::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine->GetWorldFromContextObject(GetWorldContext(), EGetWorldErrorMode::LogAndReturnNull);
	if (!World) World = GEngine->GetCurrentPlayWorld();
	return World;
}

bool UToroAsyncActionBase::IsRunningChecked()
{
	if (bTaskRunning)
	{
		UE_KLOG_WARNING(2.0f, TEXT("Async Action %s is already running"), *GetClass()->GetName())
		return true;
	}

	bTaskRunning = true;
	return false;
}
