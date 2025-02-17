// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

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
