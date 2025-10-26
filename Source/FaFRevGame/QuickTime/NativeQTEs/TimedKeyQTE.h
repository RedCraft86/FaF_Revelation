// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QuickTime/QTEInstance.h"
#include "TimedKeyQTE.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UTimedKeyQTE final : public UQTEInstance
{
	GENERATED_BODY()

public:

	UTimedKeyQTE()
		: DrainRate(1.0f), TargetKey(EKeys::F), Progress(1.0f)
	{
		WidgetClass = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
		float DrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
		FKey TargetKey;

	virtual float GetProgress() override
	{
		return Progress;
	}

	virtual void TickQTE_Implementation(const float DeltaTime) override
	{
		Progress -= DrainRate * DeltaTime;
		if (Progress < 0.0f)
		{
			MarkFinished(false);
		}
	}

	virtual void KeyPressed_Implementation(const FKey& Key) override
	{
		if (Key == TargetKey)
		{
			Progress = 1.0f;
			MarkFinished(true);
		}
	}

protected:

	float Progress;
};