// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/CustomAction.h"

void FWControlObject::RunEvent(const UObject* WorldContext)
{
	if (ActionObject)
	{
		ActionObject->ContextObject = const_cast<UObject*>(WorldContext);
		ActionObject->RunEvent();
	}
}

void FWControlObject::OnBeginPlay(const UObject* WorldContext)
{
	if (ActionObject)
	{
		ActionObject->ContextObject = const_cast<UObject*>(WorldContext);
		ActionObject->OnBeginPlay();
	}
}

void FWControlObject::OnTick(const UObject* WorldContext, const float DeltaTime)
{
	if (ActionObject && ActionObject->bCanEverTick)
	{
		ActionObject->ContextObject = const_cast<UObject*>(WorldContext);
		ActionObject->OnTick(DeltaTime);
	}
}

void FWControlObject::OnPostEditChange(const UObject* WorldContext)
{
	bCanEverTick = ActionObject ? ActionObject->bCanEverTick : false;
}
