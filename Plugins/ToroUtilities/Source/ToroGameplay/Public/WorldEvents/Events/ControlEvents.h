// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldEvents/WorldEventBase.h"
#include "ControlEvents.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Control", meta = (Hidden))
struct TOROGAMEPLAY_API FWCBase : public FWorldEventBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Wait, Then Do")
struct TOROGAMEPLAY_API FWCDelay final : public FWCBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Delay", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Delay;

	UPROPERTY(EditAnywhere, Category = "Delay")
		bool bRetriggerable;
	
	UPROPERTY(EditAnywhere, Category = "Delay", meta = (ExcludeBaseStruct, ShowTreeView))
		TArray<TInstancedStruct<FWorldEventBase>> Events;

	FWCDelay() : Delay(0.25f), bRetriggerable(false) {}

protected:

	FTimerHandle Handle;
	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnConstruction(const UObject* WorldContext, const bool bEditorTime) override;
};

USTRUCT(BlueprintType, DisplayName = "Flip Flop")
struct TOROGAMEPLAY_API FWCFlipFlop final : public FWCBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "FlipFlop", meta = (ExcludeBaseStruct, ShowTreeView))
		TArray<TInstancedStruct<FWorldEventBase>> Flip;

	UPROPERTY(EditAnywhere, Category = "FlipFlop", meta = (ExcludeBaseStruct, ShowTreeView))
		TArray<TInstancedStruct<FWorldEventBase>> Flop;

	FWCFlipFlop() : bIsA(false) {}

protected:

	bool bIsA;
	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnConstruction(const UObject* WorldContext, const bool bEditorTime) override;
};

USTRUCT(BlueprintType, DisplayName = "Do Once")
struct TOROGAMEPLAY_API FWCDoOnce final : public FWCBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "DoOnce", meta = (ExcludeBaseStruct, ShowTreeView))
		TArray<TInstancedStruct<FWorldEventBase>> Events;

	FWCDoOnce() : bHasDone(false) {}

protected:

	bool bHasDone;
	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnConstruction(const UObject* WorldContext, const bool bEditorTime) override;
};