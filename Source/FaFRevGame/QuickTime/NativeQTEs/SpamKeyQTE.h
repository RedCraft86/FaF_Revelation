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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
		float DrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
		float GainAmount;

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
			Progress += GainAmount;

			OnKeyPress.Broadcast();

			if (Progress >= 1.0f)
			{
				MarkFinished(true);
			}
		}
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyPress);
	UPROPERTY(BlueprintAssignable)
		FOnKeyPress OnKeyPress;

protected:

	float Progress;
	
	virtual void UnbindDelegates() override
	{
		OnKeyPress.Clear();
	}
};