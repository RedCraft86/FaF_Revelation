// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "LightProbeManager.generated.h"

UCLASS()
class TOROFRAMEWORK_API ULightProbeManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	// TODO

private:

	virtual TStatId GetStatId() const override { return GetStatID(); }
};
