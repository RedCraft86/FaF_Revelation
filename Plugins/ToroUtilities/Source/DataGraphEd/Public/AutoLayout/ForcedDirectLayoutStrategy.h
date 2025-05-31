// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AutoLayoutStrategy.h"
#include "ForcedDirectLayoutStrategy.generated.h"

UCLASS()
class UForcedDirectLayoutStrategy final : public UAutoLayoutStrategy
{
	GENERATED_BODY()
	
public:
	
	UForcedDirectLayoutStrategy();

	virtual void Layout(UEdGraph* InGraph) override;

private:

	bool bRandomInit;
	float InitTemperature;
	float CoolDownRate;

	FBox2D LayoutOneTree(UDataNodeBase* RootNode, const FBox2D& PreTreeBound);
};