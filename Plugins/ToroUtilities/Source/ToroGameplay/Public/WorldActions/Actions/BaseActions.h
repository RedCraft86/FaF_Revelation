// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "BaseActions.generated.h"

USTRUCT(BlueprintType, DisplayName = "Delay")
struct TOROGAMEPLAY_API FWControlDelay final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Delay)
		bool bRetriggerable;
	
	UPROPERTY(EditAnywhere, Category = Delay, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Delay;
	
	UPROPERTY(EditAnywhere, Category = Delay, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Events;

	FWControlDelay() : bRetriggerable(false), Delay(0.25f) {}

protected:

	FTimerHandle Handle;
	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnBeginPlay(const UObject* WorldContext) override;
	virtual void OnTick(const UObject* WorldContext, const float DeltaTime) override;
	virtual void OnPostEditChange(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Flip Flop")
struct TOROGAMEPLAY_API FWControlFlipFlop final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = FlipFlop, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Flip;

	UPROPERTY(EditAnywhere, Category = FlipFlop, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Flop;

	FWControlFlipFlop() : bIsA(false) {}

protected:

	bool bIsA;
	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnBeginPlay(const UObject* WorldContext) override;
	virtual void OnTick(const UObject* WorldContext, const float DeltaTime) override;
	virtual void OnPostEditChange(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Do Once")
struct TOROGAMEPLAY_API FWControlDoOnce final : public FWorldActionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = DoOnce, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Events;

	FWControlDoOnce() : bHasDone(false) {}

protected:

	bool bHasDone;
	virtual void RunEvent(const UObject* WorldContext) override;
	virtual void OnBeginPlay(const UObject* WorldContext) override;
	virtual void OnTick(const UObject* WorldContext, const float DeltaTime) override;
	virtual void OnPostEditChange(const UObject* WorldContext) override;
};