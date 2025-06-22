// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "AutoLayoutBase.generated.h"

class UEdDataGraphNode;

UCLASS()
class UAutoLayoutBase : public UObject
{
	GENERATED_BODY()

public:

	UAutoLayoutBase();

	virtual void Layout(UEdGraph* InGraph) {}

protected:

	int32 MaxIteration;
	int32 OptimalDistance;
	TObjectPtr<class UDataGraphBase> Graph;
	TObjectPtr<class UEdDataGraph> EdGraph;

	static FBox2D GetNodeBound(UEdGraphNode* EdNode);
	FBox2D GetActualBounds(class UDataNodeBase* RootNode) const;
	virtual void RandomLayoutOneTree(UDataNodeBase* RootNode, const FBox2D& Bound);
};
