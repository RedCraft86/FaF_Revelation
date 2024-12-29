// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroAsyncActionBase.h"
#include "LoggingHelpers.h"

UWorld* UToroAsyncActionBase::GetWorld() const
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) World = Super::GetWorld();
	
#if WITH_EDITOR
	if (!FApp::IsGame()) return GEngine->GetCurrentPlayWorld();
	if (!IsValid(World))
	{
		UE_KLOG_WARNING(2.0f, TEXT("Cannot access world on Async Action %s. Using last resort method."), *GetClass()->GetName())
		World = GEngine->GetCurrentPlayWorld();
	}
	
	return World;
#else
	return IsValid(World) ? World : GEngine->GetCurrentPlayWorld();
#endif
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
