// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "ObjectiveManager.h"
#include "SaveObjects/GameSaveObject.h"
#include "ObjectiveDatabase.h"
#include "FaFRevSettings.h"
#include "ObjectiveActor.h"

UObjectiveManager::UObjectiveManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectiveManager::CompleteObjective(const FGameplayTag& Tag)
{
	for (FObjectiveMetadata& Objective : Objectives)
	{
		if (!Objective.bCompleted && Objective.Tag == Tag)
		{
			Objective.bCompleted = true;
			GameSave->AddFlag(Tag);
			return;
		}
	}
}

void UObjectiveManager::CompleteObjective(const AObjectiveActor* Actor)
{
	for (FObjectiveMetadata& Objective : Objectives)
	{
		if (!Objective.bCompleted && Objective.Actor == Actor)
		{
			Objective.bCompleted = true;
			GameSave->AddFlag(Objective.Tag);
			return;
		}
	}
}

void UObjectiveManager::StartObjectives(const FGameplayTagContainer& NewObjectives)
{
	Objectives.Empty();
	for (const FGameplayTag& Tag : NewObjectives)
	{
		if (const FObjectiveEntry* Objective = UObjectiveDatabase::GetEntry(Tag))
		{
			AObjectiveActor* Instance = Objective->Instance.LoadSynchronous();
			if (Objective->Instance.IsNull() || Instance)
			{
				const bool bComplete = GameSave->HasFlag(Tag);
				Objectives.Emplace(Tag, bComplete, Instance);
				if (Instance)
				{
					if (bComplete)
					{
						Instance->MarkCompleted(false);
					}
					else
					{
						Instance->StartObjective();
					}
				}
			}
		}
	}
}
