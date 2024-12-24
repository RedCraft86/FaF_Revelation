// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphNodeFactory.h"
#include "DataGraphEdEdge.h"
#include "DataGraphEdNode.h"
#include "SDataGraphEdge.h"
#include "SDataGraphNode.h"

TSharedPtr<SGraphNode> FDataGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UDataGraphEdNode* EdNode_GraphNode = Cast<UDataGraphEdNode>(Node))
	{
		return SNew(SDataGraphNode, EdNode_GraphNode);
	}
	
	if (UDataGraphEdEdge* EdNode_Edge = Cast<UDataGraphEdEdge>(Node))
	{
		return SNew(SDataGraphEdge, EdNode_Edge);
	}
	
	return FGraphPanelNodeFactory::CreateNode(Node);
}
