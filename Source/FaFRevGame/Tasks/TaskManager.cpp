// Copyright (C) RedCraft86. All Rights Reserved.

#include "TaskManager.h"

ATaskManager::ATaskManager()
{
	PrimaryActorTick.bCanEverTick = false;
#if WITH_EDITOR
	EditorShapes = CreateEditorOnlyDefaultSubobject<UEditorShapeComponent>("EditorShapes");
	bRunConstructionScriptOnDrag = true;
#endif
}

void ATaskManager::OnTaskComplete()
{
	if (FIntPoint Progress = FIntPoint(0, TaskActors.Num()); Progress.Y > 0)
	{
		for (const TObjectPtr<ATaskActor>& Actor : TaskActors)
		{
			if (Actor->IsCompleted()) Progress.X++;
		}
		OnTaskProgress.Broadcast(Progress);
		OnTaskProgressBP.Broadcast(Progress);
	}
}

void ATaskManager::BeginPlay()
{
	Super::BeginPlay();
	for (auto It = TaskActors.CreateIterator(); It; ++It)
	{
		ATaskActor* Actor = *It;
		if (Actor && Actor->IsValidTaskActor())
		{
			Actor->OnTaskComplete.AddUObject(this, &ATaskManager::OnTaskComplete);
		}
		else
		{
			It.RemoveCurrent();
		}
	}
}

#if WITH_EDITOR
void ATaskManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (EditorShapes)
	{
		FWirePointData RootData;
		RootData.Location = Transform.GetTranslation();
		RootData.bRenderOnTopOfEverything = true;
		RootData.Color = FLinearColor::Green;
		RootData.Size = 5.0f;

		EditorShapes->WireLines.Empty(TaskActors.Num());
		EditorShapes->WirePoints.Empty(TaskActors.Num() + 1);

		EditorShapes->WirePoints.Add("Root", RootData);
		for (const TObjectPtr<ATaskActor>& Actor : TaskActors)
		{
			if (!Actor) continue;

			FWireLineData LineData;
			LineData.Thickness = 1.0f;
			LineData.bRenderOnTopOfEverything = true;
			LineData.End = Actor->GetActorLocation();
			LineData.Start = Transform.GetTranslation();
			LineData.Color = FLinearColor(0.0f, 0.7f, 0.8f);
			EditorShapes->WireLines.Add(Actor->GetFName(), LineData);

			FWirePointData PointData;
			PointData.Size = 2.5f;
			PointData.Color = FLinearColor::Red;
			PointData.bRenderOnTopOfEverything = true;
			PointData.Location = Actor->GetActorLocation();
			EditorShapes->WirePoints.Add(Actor->GetFName(), PointData);
		}
	}
}
#endif