// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AutoLayoutBase.h"
#include "DirectLayoutStrategy.generated.h"

UCLASS()
class UDirectLayoutStrategy final : public UAutoLayoutBase
{
	GENERATED_BODY()

public:

	UDirectLayoutStrategy()
		: bRandomInit(false), CoolDownRate(10.0f), InitTemperature(10.0f)
	{}

	virtual void Layout(UEdGraph* InGraph) override;

private:

	bool bRandomInit;
	float CoolDownRate;
	float InitTemperature;

	FBox2D LayoutOneTree(UDataNodeBase* RootNode, const FBox2D& PreTreeBound);
};
