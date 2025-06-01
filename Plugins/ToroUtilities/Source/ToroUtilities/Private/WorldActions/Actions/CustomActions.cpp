// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/CustomActions.h"

void FWACustomObject::RunAction()
{
	if (ActionObject)
	{
		ActionObject->WorldContext = const_cast<UObject*>(GetWorldContext());
		ActionObject->RunAction();
	}
}

void FWACustomObject::OnBeginPlay()
{
	if (ActionObject)
	{
		ActionObject->WorldContext = const_cast<UObject*>(GetWorldContext());
		ActionObject->OnBeginPlay();
	}
}

void FWACustomObject::OnTick(const float DeltaTime)
{
	if (ActionObject && ActionObject->bCanEverTick)
	{
		ActionObject->WorldContext = const_cast<UObject*>(GetWorldContext());
		ActionObject->OnTick(DeltaTime);
	}
}

void FWACustomObject::OnPostEditChange()
{
	bShouldTick = false;
	if (ActionObject)
	{
		ActionObject->WorldContext = const_cast<UObject*>(GetWorldContext());
		ActionObject->PostActionUpdate();
		if (!bShouldTick)
			bShouldTick = ActionObject->bCanEverTick;
	}
}
