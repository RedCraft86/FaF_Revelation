// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InstancedStruct.h"
#include "WorldEventBase.generated.h"

UENUM(BlueprintType, DisplayName = "Event Change Type")
enum class EWorldEventChangeType : uint8
{
	Set,
	Toggle
};

/* Prefixes
 * WC - World Control: For flow control types, like a delay.
 * WE - World Event: For regular tasks.
 */
USTRUCT(BlueprintInternalUseOnly)
struct TOROGAMEPLAY_API FWorldEventBase
{
	GENERATED_BODY()

	FWorldEventBase() : bRequiresTick(false), bRunConstructionBeforeBeginPlay(false) {}
	virtual ~FWorldEventBase() = default;
		
	bool bRequiresTick;
	bool bRunConstructionBeforeBeginPlay;
	
	virtual void RunEvent(const UObject* WorldContext) {}
	virtual void OnConstruction(const UObject* WorldContext, const bool bEditorTime) {}
	virtual void OnTick(const UObject* WorldContext, const float DeltaTime) {}
	virtual void OnBeginPlay(const UObject* WorldContext)
	{
		if (bRunConstructionBeforeBeginPlay)
		{
			OnConstruction(WorldContext, false);
		}
	}

protected:

	static inline uint8 LatentID = 0;
	static FLatentActionInfo GetLatentInfo()
	{
		FLatentActionInfo Info;
		Info.Linkage = 0;
		Info.UUID = LatentID++;
		Info.CallbackTarget = nullptr;
		Info.ExecutionFunction = NAME_None;
		return Info;
	}
};