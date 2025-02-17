// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

/** Unused for Fredbear and Friends: Revelation */

#if 0
#include "Subsystems/WorldSubsystem.h"
#include "GameSectionManager.generated.h"

UCLASS(BlueprintType)
class TORORUNTIME_API UGameSectionManager final : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
};
#endif