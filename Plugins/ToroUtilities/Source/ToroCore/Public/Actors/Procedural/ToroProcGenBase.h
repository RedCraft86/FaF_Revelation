// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ToroProcGenBase.generated.h"

UENUM(BlueprintType)
enum class EGeneratorLoopMode : uint8
{
	Forward,
	Reverse,
	Random
};

UCLASS(Abstract)
class TOROCORE_API AToroProcGenBase : public AActor
{
	GENERATED_BODY()

	// TODO
};
