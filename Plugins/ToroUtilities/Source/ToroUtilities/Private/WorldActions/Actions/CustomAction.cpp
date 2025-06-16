// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/CustomAction.h"

bool FWACustomObject::ShouldTick() const
{
	return ActionObject && ActionObject->bCanEverTick;
}

void FWACustomObject::RunAction()
{
	if (ActionObject)
	{
		ActionObject->WorldContext = GetMutableContext();
		ActionObject->RunAction();
	}
}

void FWACustomObject::OnBeginPlay()
{
	if (ActionObject)
	{
		ActionObject->WorldContext = GetMutableContext();
		ActionObject->OnBeginPlay();
	}
}

void FWACustomObject::OnTick(const float DeltaTime)
{
	if (ActionObject && ActionObject->bCanEverTick)
	{
		ActionObject->WorldContext = GetMutableContext();
		ActionObject->OnTick(DeltaTime);
	}
}

void FWACustomObject::OnConstruction()
{
	if (ActionObject)
	{
		ActionObject->WorldContext = const_cast<UObject*>(GetWorldContext());
		ActionObject->OnConstruction();
	}
}