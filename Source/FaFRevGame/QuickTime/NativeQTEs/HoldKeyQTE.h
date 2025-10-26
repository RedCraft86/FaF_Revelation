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
		: DrainRate(0.2f), GainRate(0.5f), TargetKey(EKeys::F), bPressed(false), Progress(0.0f)
	{
		WidgetClass = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
		float DrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
		float GainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
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

			if (!bPressed)
			{
				bPressed = true;
				OnKeyPress.Broadcast(bPressed);
			}

			if (Progress >= 1.0f)
			{
				MarkFinished(true);
			}
		}
		else
		{
			Progress -= DrainRate * DeltaTime;

			if (bPressed)
			{
				bPressed = false;
				OnKeyPress.Broadcast(bPressed);
			}

			if (Progress < -10.0f)
			{
				MarkFinished(false);
			}
		}
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyPress, const bool, bPressed);
	UPROPERTY(BlueprintAssignable)
		FOnKeyPress OnKeyPress;

protected:

	bool bPressed;
	float Progress;
	
	virtual void UnbindDelegates() override
	{
		OnKeyPress.Clear();
	}
};