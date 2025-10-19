// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetterMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "TaskManager.generated.h"

UCLASS()
class FAFREVGAME_API UTaskManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	UTaskManager() {}

	WORLD_SUBSYSTEM_GETTER(UTaskManager)

private:


	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
};
