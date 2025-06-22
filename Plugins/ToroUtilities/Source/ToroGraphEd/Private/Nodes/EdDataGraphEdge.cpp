// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/EdDataGraphEdge.h"
#include "Nodes/EdDataGraphNode.h"

UEdDataGraphEdge::UEdDataGraphEdge()
{
	Graph = nullptr;
	GraphEdge = nullptr;
	bCanRenameNode = true;
}

UEdDataGraphNode* UEdDataGraphEdge::GetEndNode()
{
	return Pins[1]->LinkedTo.IsEmpty() ? nullptr : Cast<UEdDataGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
}

UEdDataGraphNode* UEdDataGraphEdge::GetStartNode()
{
	return Pins[0]->LinkedTo.IsEmpty() ? nullptr : Cast<UEdDataGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
}

void UEdDataGraphEdge::CreateConnections(const UEdDataGraphNode* Start, const UEdDataGraphNode* End)
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

void UEdDataGraphEdge::PrepareForCopying()
{
	GraphEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdDataGraphEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;
	
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

void UEdDataGraphEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.IsEmpty())
	{
		Modify();
		if (UEdGraph* ParentGraph = GetGraph()) ParentGraph->Modify();
		DestroyNode();
	}
}

FText UEdDataGraphEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GraphEdge ? GraphEdge->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}
