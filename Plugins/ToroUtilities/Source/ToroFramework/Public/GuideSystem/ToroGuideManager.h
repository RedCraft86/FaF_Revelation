// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "ToroGuideManager.generated.h"

UCLASS()
class TOROFRAMEWORK_API UToroGuideManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	// TODO

private:

	virtual TStatId GetStatId() const override { return GetStatID(); }
};
