// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "ObjectiveActor.h"
#include "ObjectiveManager.h"

AObjectiveActor::AObjectiveActor()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AObjectiveActor::MarkCompleted_Implementation(const bool bNotifyManager)
{
	if (bNotifyManager)
	{
		if (UObjectiveManager* Manager = UObjectiveManager::Get(this))
		{
			Manager->CompleteObjective(this);
		}
	}
}
