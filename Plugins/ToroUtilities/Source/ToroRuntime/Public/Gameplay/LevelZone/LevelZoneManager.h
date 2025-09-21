// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Helpers/ClassGetterMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelZoneManager.generated.h"

UCLASS()
class TORORUNTIME_API ULevelZoneManager final : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	ULevelZoneManager() {}

	WORLD_SUBSYSTEM_GETTER(ULevelZoneManager);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
};
