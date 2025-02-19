// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once


#include "ClassGetterHelpers.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AchievementSystem.generated.h"

UCLASS(BlueprintType)
class TORORUNTIME_API UAchievementSystem final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UAchievementSystem() {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UAchievementSystem);

private:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};
