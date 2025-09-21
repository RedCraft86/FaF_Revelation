// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetterMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "TutorialManager.generated.h"

UCLASS()
class TORORUNTIME_API UTutorialManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	UTutorialManager() {}

	WORLD_SUBSYSTEM_GETTER(UTutorialManager);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:

	virtual TStatId GetStatId() const override { return GetStatID(); }
};
