// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/DataGraphNodeFactory.h"
#include "Nodes/EdDataGraphNode.h"
#include "Nodes/EdDataGraphEdge.h"
#include "Nodes/SDataGraphEdge.h"
#include "Nodes/SDataGraphNode.h"

TSharedPtr<SGraphNode> FDataGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UEdDataGraphNode* EdNode_GraphNode = Cast<UEdDataGraphNode>(Node))
	{
		return SNew(SDataGraphNode, EdNode_GraphNode);
	}
	
	if (UEdDataGraphEdge* EdNode_Edge = Cast<UEdDataGraphEdge>(Node))
	{
		return SNew(SDataGraphEdge, EdNode_Edge);
	}
	
	return FGraphPanelNodeFactory::CreateNode(Node);
}
