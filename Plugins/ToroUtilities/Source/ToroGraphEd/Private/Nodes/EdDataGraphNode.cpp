// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/EdDataGraphNode.h"
#include "Nodes/SDataGraphNode.h"
#include "Graph/EdDataGraph.h"

UEdDataGraphNode::UEdDataGraphNode(): SNode(nullptr)
{
	GraphNode = nullptr;
	bCanRenameNode = true;
}

FText UEdDataGraphNode::GetTextID() const
{
	return GraphNode ? FText::FromString(GraphNode->NodeID.ToString()) : INVTEXT("Unknown ID");
}

FText UEdDataGraphNode::GetDescription() const
{
	return GraphNode ? GraphNode->GetNodeDescription() : INVTEXT("Unknown Node");
}

FVector2D UEdDataGraphNode::GetNodeSize() const
{
	return SNode->GetCachedGeometry().GetLocalSize();
}

UEdDataGraph* UEdDataGraphNode::GetEdDataGraph() const
{
	return Cast<UEdDataGraph>(GetGraph());
}

FLinearColor UEdDataGraphNode::GetBackgroundColor() const
{
	return GraphNode ? GraphNode->BackgroundColor : FLinearColor::Black;
}

void UEdDataGraphNode::PrepareForCopying()
{
	GraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdDataGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

void UEdDataGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);
	if (!FromPin && GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
	{
		FromPin->GetOwningNode()->NodeConnectionListChanged();
	}
}

FText UEdDataGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GraphNode ? GraphNode->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}
