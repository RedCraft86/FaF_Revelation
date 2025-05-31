// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphEdEdge.h"
#include "DataGraphEdNode.h"

#include "DataEdgeBase.h"

UDataGraphEdEdge::UDataGraphEdEdge()
{
	Graph = nullptr;
	GraphEdge = nullptr;
	bCanRenameNode = true;
}

void UDataGraphEdEdge::SetEdge(UDataEdgeBase* Edge)
{
	GraphEdge = Edge;
}

void UDataGraphEdEdge::CreateConnections(UDataGraphEdNode* Start, UDataGraphEdNode* End)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	Start->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(Start->GetOutputPin());

	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	End->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(End->GetInputPin());
}

void UDataGraphEdEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;
	
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UDataGraphEdEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GraphEdge ? GraphEdge->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}

void UDataGraphEdEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.IsEmpty())
	{
		Modify();
		if (UEdGraph* ParentGraph = GetGraph()) ParentGraph->Modify();
		DestroyNode();
	}
}

void UDataGraphEdEdge::PrepareForCopying()
{
	GraphEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

UDataGraphEdNode* UDataGraphEdEdge::GetStartNode()
{
	return Pins[0]->LinkedTo.IsEmpty() ? nullptr : Cast<UDataGraphEdNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
}

UDataGraphEdNode* UDataGraphEdEdge::GetEndNode()
{
	return Pins[1]->LinkedTo.IsEmpty() ? nullptr : Cast<UDataGraphEdNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
}
