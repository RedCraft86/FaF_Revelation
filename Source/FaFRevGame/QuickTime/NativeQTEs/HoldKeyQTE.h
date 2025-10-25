// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QuickTime/QTEInstance.h"
#include "HoldKeyQTE.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UHoldKeyQTE final : public UQTEInstance
{
	GENERATED_BODY()

public:

	UHoldKeyQTE()
		: DrainRate(0.2f), GainRate(0.5f), TargetKey(EKeys::F), Progress(0.0f)
	{
		WidgetClass = nullptr;
	}

	UPROPERTY(EditAnywhere, Category = Settings)
		float DrainRate;

	UPROPERTY(EditAnywhere, Category = Settings)
		float GainRate;

	UPROPERTY(EditAnywhere, Category = Settings)
		FKey TargetKey;

	virtual float GetProgress() override
	{
		return Progress;
	}

	virtual void TickQTE_Implementation(const float DeltaTime) override
	{
		if (Controller && Controller->IsInputKeyDown(TargetKey))
		{
			Progress += GainRate * DeltaTime;
			if (Progress >= 1.0f)
			{
				MarkFinished(true);
			}
		}
		else
		{
			Progress -= DrainRate * DeltaTime;
			if (Progress < -10.0f)
			{
				MarkFinished(false);
			}
		}
	}

protected:

	float Progress;
};