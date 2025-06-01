// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ECFHandle.h"
#include "WorldActions/WorldActionBase.h"
#include "FlowActions.generated.h"

USTRUCT(BlueprintType, DisplayName = "Delay")
struct TOROUTILITIES_API FWAFlowDelay final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Delay)
		bool bRetriggerable;
	
	UPROPERTY(EditAnywhere, Category = Delay, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Delay;
	
	UPROPERTY(EditAnywhere, Category = Delay, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	FWAFlowDelay(): bRetriggerable(false), Delay(0.25f) {}

protected:

	FECFHandle DelayHandle;
	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnTick(const float DeltaTime) override;
	virtual void OnPostEditChange() override;
};

USTRUCT(BlueprintType, DisplayName = "Do Once")
struct TOROUTILITIES_API FWAFlowDoOnce final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = DoOnce, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	FWAFlowDoOnce(): bHasDone(false) {}

protected:

	bool bHasDone;
	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnTick(const float DeltaTime) override;
	virtual void OnPostEditChange() override;
};

USTRUCT(BlueprintType, DisplayName = "Flip Flop")
struct TOROUTILITIES_API FWAFlowFlipFlop final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = FlipFlop, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Flip;

	UPROPERTY(EditAnywhere, Category = FlipFlop, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Flop;

	FWAFlowFlipFlop(): bIsA(false) {}

protected:

	bool bIsA;
	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
	virtual void OnTick(const float DeltaTime) override;
	virtual void OnPostEditChange() override;
};