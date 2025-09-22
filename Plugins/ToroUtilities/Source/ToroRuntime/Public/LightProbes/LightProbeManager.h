// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetterMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "LightProbeManager.generated.h"

UCLASS()
class TORORUNTIME_API ULightProbeManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	ULightProbeManager() {}

	WORLD_SUBSYSTEM_GETTER(ULightProbeManager);

	// TODO

private:

	virtual TStatId GetStatId() const override { return GetStatID(); }
};
