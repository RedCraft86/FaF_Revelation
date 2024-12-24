// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "AutoLayoutStrategy.generated.h"

class UDataGraphEdNode;

UCLASS(Abstract)
class UAutoLayoutStrategy : public UObject
{
	GENERATED_BODY()
	
public:
	
	UAutoLayoutStrategy();

	virtual void Layout(UEdGraph* InGraph) {}

protected:
	
	int32 MaxIteration;
	int32 OptimalDistance;
	TObjectPtr<class UDataGraphEdGraph> EdGraph;
	TObjectPtr<class UDataGraphBase> Graph;
	
	FBox2D GetNodeBound(UEdGraphNode* EdNode) const;
	int32 GetNodeWidth(const UDataGraphEdNode* EdNode) const;
	int32 GetNodeHeight(const UDataGraphEdNode* EdNode) const;
	FBox2D GetActualBounds(class UDataNodeBase* RootNode) const;
	virtual void RandomLayoutOneTree(class UDataNodeBase* RootNode, const FBox2D& Bound);
};
