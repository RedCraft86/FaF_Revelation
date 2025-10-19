// Copyright (C) RedCraft86. All Rights Reserved.

#include "TaskManager.h"

bool UTaskManager::IsTickable() const
{
	return Super::IsTickable();
}

void UTaskManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UTaskManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

bool UTaskManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

bool UTaskManager::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return Super::DoesSupportWorldType(WorldType);
}
