// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphEdNode.h"
#include "DataGraphEdGraph.h"

#include "DataNodeBase.h"

UDataGraphEdNode::UDataGraphEdNode()
{
	SNode = nullptr;
	GraphNode = nullptr;
	bCanRenameNode = true;
}

void UDataGraphEdNode::SetDataGraphNode(UDataNodeBase* InNode)
{
	GraphNode = InNode;
}

UDataGraphEdGraph* UDataGraphEdNode::GetDataGraphEdGraph() const
{
	return Cast<UDataGraphEdGraph>(GetGraph());
}

void UDataGraphEdNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

FText UDataGraphEdNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GraphNode ? GraphNode->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}

void UDataGraphEdNode::PrepareForCopying()
{
	GraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UDataGraphEdNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);
	if (!FromPin) return;
	
	if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
	{
		FromPin->GetOwningNode()->NodeConnectionListChanged();
	}
}

FText UDataGraphEdNode::GetDescription() const
{
	return GraphNode ? GraphNode->GetNodeDescription() : INVTEXT("Unknown Node");
}

FText UDataGraphEdNode::GetIdentifierText() const
{
	return GraphNode ? FText::FromString(GraphNode->NodeID.ToString()) : INVTEXT("Unknown ID");
}

FLinearColor UDataGraphEdNode::GetBackgroundColor() const
{
	return GraphNode ? GraphNode->BackgroundColor : FLinearColor::Black;
}
