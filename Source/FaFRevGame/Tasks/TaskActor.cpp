// Copyright (C) RedCraft86. All Rights Reserved.

#include "TaskActor.h"

ATaskActor::ATaskActor(): Quantity(1), bCompleted(false)
{
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITOR
	Task = LoadObject<UNarrativeDataTask>(nullptr, TEXT("/Narrative/DefaultTasks/LooseTask.LooseTask"));
#endif
}

void ATaskActor::MarkTaskComplete()
{
	if (Narrative && IsValidTaskActor())
	{
		SetEnabled(false);
		bCompleted = true;
		Narrative->CompleteNarrativeDataTask(Task, Argument, Quantity);

		OnTaskComplete.Broadcast();
		OnTaskCompleteBP.Broadcast();
	}
}

void ATaskActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		Narrative = UNarrativeManager::Get(this);
	});
}

#if WITH_EDITOR
void ATaskActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Quantity == 0) Quantity = 1;
}
#endif