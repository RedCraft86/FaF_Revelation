// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QuickTime/QTEInstance.h"
#include "SpamKeyQTE.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API USpamKeyQTE final : public UQTEInstance
{
	GENERATED_BODY()

public:

	USpamKeyQTE()
		: DrainRate(0.5f), GainAmount(0.1f), TargetKey(EKeys::F), Progress(0.5f)
	{
		WidgetClass = nullptr;
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		float DrainRate;

	UPROPERTY(EditAnywhere, Category = Settings)
		float GainAmount;

	UPROPERTY(EditAnywhere, Category = Settings)
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
			Progress += GainAmount;
			if (Progress >= 1.0f)
			{
				MarkFinished(true);
			}
		}
	}

protected:

	float Progress;
};