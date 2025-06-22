// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/DataGraphSchema.h"

#include "DataGraphBase.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/DataGraphConnection.h"
#include "Graph/EdDataGraph.h"
#include "Nodes/EdDataGraphNode.h"
#include "Nodes/EdDataGraphEdge.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

int32 UEdDataGraphSchema::CurrentCacheRefreshID = 0;

class FNodeVisitorCycleChecker
{
public:

	bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
	{
		VisitedNodes.Add(StartNode);
		return TraverseNodes(EndNode);
	}

private:
	
	bool TraverseNodes(UEdGraphNode* Node)
	{
		VisitedNodes.Add(Node);
		for (const UEdGraphPin* MyPin : Node->Pins)
		{
			if (MyPin->Direction == EGPD_Output)
			{
				for (const UEdGraphPin* OtherPin : MyPin->LinkedTo)
				{
					UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
					if (VisitedNodes.Contains(OtherNode)) return false;
					if (!FinishedNodes.Contains(OtherNode))
					{
						if (!TraverseNodes(OtherNode)) return false;
					}
				}
			}
		}

		VisitedNodes.Remove(Node);
		FinishedNodes.Add(Node);
		return true;
	}
	
	TSet<UEdGraphNode*> VisitedNodes;
	TSet<UEdGraphNode*> FinishedNodes;
};

UEdGraphNode* FDataGraphAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2f& Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;
	if (NodeTemplate)
	{
		const FScopedTransaction Transaction(LOCTEXT("NewNode", "Data Graph Editor: New Node"));
		if (FromPin) FromPin->Modify();
		ParentGraph->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->GraphNode->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FDataGraphAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

UEdGraphNode* FDataGraphAction_NewEdge::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2f& Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;
	if (NodeTemplate)
	{
		const FScopedTransaction Transaction(LOCTEXT("NewEdge", "Data Graph Editor: New Edge"));
		if (FromPin) FromPin->Modify();
		ParentGraph->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->GraphEdge->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}
	
	return ResultNode;
}

void FDataGraphAction_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

void UEdDataGraphSchema::GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin)
{
	TMap<FString, uint32> LinkTitleCount;
	FToolMenuSection& Section = Menu->FindOrAddSection("DataGraphAssetGraphSchemaPinActions");
	for (TArray<UEdGraphPin*>::TConstIterator Links(InGraphPin->LinkedTo); Links; ++Links)
	{
		const UEdGraphPin* Pin = *Links;
		FString TitleString = Pin->GetOwningNode()->GetNodeTitle(ENodeTitleType::ListView).ToString();
		FText Title = FText::FromString(TitleString);
		if (Pin->PinName != TEXT(""))
		{
			TitleString = FString::Printf(TEXT("%s (%s)"), *TitleString, *Pin->PinName.ToString());

			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), Title);
			Args.Add(TEXT("PinName"), Pin->GetDisplayName());
			Title = FText::Format(LOCTEXT("BreakDescPin", "{NodeTitle} ({PinName})"), Args);
		}

		uint32& Count = LinkTitleCount.FindOrAdd(TitleString);

		FText Description;
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), Title);
		Args.Add(TEXT("NumberOfNodes"), Count);

		if (Count == 0)
		{
			Description = FText::Format(LOCTEXT("BreakDesc", "Break link to {NodeTitle}"), Args);
		}
		else
		{
			Description = FText::Format(LOCTEXT("BreakDescMulti", "Break link to {NodeTitle} ({NumberOfNodes})"), Args);
		}
		Count++;

		Section.AddMenuEntry(NAME_None, Description, Description, FSlateIcon(), FUIAction(
			FExecuteAction::CreateUObject(this, &UEdDataGraphSchema::BreakSinglePinLink, InGraphPin, *Links)));
	}
}

EGraphType UEdDataGraphSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

void UEdDataGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	UDataGraphBase* Graph = CastChecked<UDataGraphBase>(ContextMenuBuilder.CurrentGraph->GetOuter());
	if (!Graph->CompatibleNode) return;

	TSet<TSubclassOf<UDataNodeBase>> Visited;
	FText Desc = Graph->CompatibleNode.GetDefaultObject()->ContextMenuName;
	const FText AddToolTip = LOCTEXT("NewNodeTooltip", "Add node here");
	
	if (Desc.IsEmpty())
	{
		FString Title = Graph->CompatibleNode->GetName();
		Title.RemoveFromEnd("_C");
		Desc = FText::FromString(Title);
	}

	if (!Graph->CompatibleNode->HasAnyClassFlags(CLASS_Abstract))
	{
		const TSharedPtr<FDataGraphAction_NewNode> Action(new FDataGraphAction_NewNode(LOCTEXT("NodeAction", "Data Graph Node"), Desc, AddToolTip, 0));
		Action->NodeTemplate = NewObject<UEdDataGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
		Action->NodeTemplate->GraphNode = NewObject<UDataNodeBase>(Action->NodeTemplate, Graph->CompatibleNode);
		Action->NodeTemplate->GraphNode->OwningGraph = Graph;
		ContextMenuBuilder.AddAction(Action);

		Visited.Add(Graph->CompatibleNode);
	}

	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(Graph->CompatibleNode) && !It->HasAnyClassFlags(CLASS_Abstract) && !Visited.Contains(*It))
		{
			TSubclassOf<UDataNodeBase> NodeType = *It;
			if (It->GetName().StartsWith("REINST") || It->GetName().StartsWith("SKEL")) continue;
			if (!Graph->GetClass()->IsChildOf(NodeType.GetDefaultObject()->CompatibleGraph)) continue;

			Desc = NodeType.GetDefaultObject()->ContextMenuName;

			if (Desc.IsEmpty())
			{
				FString Title = NodeType->GetName();
				Title.RemoveFromEnd("_C");
				Desc = FText::FromString(Title);
			}

			TSharedPtr<FDataGraphAction_NewNode> Action(new FDataGraphAction_NewNode(LOCTEXT("NodeAction", "Data Graph Node"), Desc, AddToolTip, 0));
			Action->NodeTemplate = NewObject<UEdDataGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
			Action->NodeTemplate->GraphNode = NewObject<UDataNodeBase>(Action->NodeTemplate, NodeType);
			Action->NodeTemplate->GraphNode->OwningGraph = Graph;
			ContextMenuBuilder.AddAction(Action);

			Visited.Add(NodeType);
		}
	}
}

void UEdDataGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if (Context->Pin)
	{
		FToolMenuSection& Section = Menu->AddSection("DataGraphSchemaActions", LOCTEXT("PinActionsMenuHeader", "Pin Actions"));
		if (!Context->Pin->LinkedTo.IsEmpty())
		{
			Section.AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);
			if (Context->Pin->LinkedTo.Num() > 1)
			{
				Section.AddSubMenu("BreakLinkTo", LOCTEXT("BreakLinkTo", "Break Link To..."), LOCTEXT("BreakSpecificLinks", "Break a specific link..."),
					FNewToolMenuDelegate::CreateUObject((UEdDataGraphSchema* const)this, &UEdDataGraphSchema::GetBreakLinkToSubMenuActions, const_cast<UEdGraphPin*>(Context->Pin)));
			}
			else
			{
				((UEdDataGraphSchema* const)this)->GetBreakLinkToSubMenuActions(Menu, const_cast<UEdGraphPin*>(Context->Pin));
			}
		}
	}
	else if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("DataGraphSchemaActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);

		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}

	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UEdDataGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Can't connect node to itself"));
	}

	const UEdGraphPin* Out = A;
	const UEdGraphPin* In = B;

	const UEdDataGraphNode* EdNode_Out = Cast<UEdDataGraphNode>(Out->GetOwningNode());
	const UEdDataGraphNode* EdNode_In = Cast<UEdDataGraphNode>(In->GetOwningNode());
	if (EdNode_Out == nullptr || EdNode_In == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinError", "Not a valid UEdDataGraphNode"));
	}

	bool bAllowCycles = false;
	if (const UEdDataGraph* EdGraph = Cast<UEdDataGraph>(Out->GetOwningNode()->GetGraph()))
	{
		bAllowCycles = EdGraph->GetGraph()->bCanBeCyclical;
	}
	
	FNodeVisitorCycleChecker CycleChecker;
	if (!bAllowCycles && !CycleChecker.CheckForLoop(Out->GetOwningNode(), In->GetOwningNode()))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorCycle", "Can't create a graph cycle"));
	}

	FString ErrorMessage;
	if (!EdNode_Out->GraphNode->CanConnectTo(EdNode_In->GraphNode, EdNode_Out->GetOutputPin()->LinkedTo.Num(), &ErrorMessage))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}
	if (!EdNode_In->GraphNode->CanConnectFrom(EdNode_Out->GraphNode, EdNode_In->GetInputPin()->LinkedTo.Num(), &ErrorMessage))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}
	
	if (EdNode_Out->GraphNode->OwningGraph->CompatibleEdge)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, LOCTEXT("PinConnect", "Connect nodes with edge"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
}

bool UEdDataGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	const UEdDataGraphNode* NodeA = Cast<UEdDataGraphNode>(A->GetOwningNode());
	const UEdDataGraphNode* NodeB = Cast<UEdDataGraphNode>(B->GetOwningNode());
	for (const UEdGraphPin* TestPin : NodeA->GetOutputPin()->LinkedTo)
	{
		UEdGraphNode* ChildNode = TestPin->GetOwningNode();
		if (UEdDataGraphEdge* EdNode_Edge = Cast<UEdDataGraphEdge>(ChildNode))
		{
			ChildNode = EdNode_Edge->GetEndNode();
		}

		if (ChildNode == NodeB) return false;
	}

	if (NodeA && NodeB)
	{
		Super::TryCreateConnection(NodeA->GetOutputPin(), NodeB->GetInputPin());
		return true;
	}
	
	return false;
}

bool UEdDataGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UEdDataGraphNode* NodeA = Cast<UEdDataGraphNode>(A->GetOwningNode());
	UEdDataGraphNode* NodeB = Cast<UEdDataGraphNode>(B->GetOwningNode());
	if (!NodeA || !NodeA->GetOutputPin() || !NodeB || !NodeB->GetInputPin()) return false;

	const UDataGraphBase* Graph = NodeA->GraphNode->OwningGraph;
	const FVector2D InitPos((NodeA->NodePosX + NodeB->NodePosX) / 2, (NodeA->NodePosY + NodeB->NodePosY) / 2);

	FDataGraphAction_NewEdge Action;
	Action.NodeTemplate = NewObject<UEdDataGraphEdge>(NodeA->GetGraph());
	Action.NodeTemplate->SetEdge(NewObject<UDataEdgeBase>(Action.NodeTemplate, Graph->CompatibleEdge));
	UEdDataGraphEdge* EdgeNode = Cast<UEdDataGraphEdge>(Action.PerformAction(NodeA->GetGraph(), nullptr, FVector2f(InitPos), false));
	EdgeNode->CreateConnections(NodeA, NodeB);

	return true;
}

FConnectionDrawingPolicy* UEdDataGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
	float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	return new FDataGraphConnection(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

FLinearColor UEdDataGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

void UEdDataGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakNodeLinks", "Break Node Links"));
	Super::BreakNodeLinks(TargetNode);
}

void UEdDataGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

void UEdDataGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakSinglePinLink", "Break Pin Link"));
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UEdDataGraphSchema::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName,
	const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	const UEdDataGraphNode* EdNode = Cast<UEdDataGraphNode>(InTargetNode);
	switch (InSourcePinDirection)
	{
	case EGPD_Input: return EdNode->GetOutputPin();
	case EGPD_Output: return EdNode->GetInputPin();
	default: return nullptr;
	}
}

bool UEdDataGraphSchema::SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType,
	EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return Cast<UEdDataGraphNode>(InTargetNode) != nullptr;
}

bool UEdDataGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return CurrentCacheRefreshID != InVisualizationCacheID;
}

int32 UEdDataGraphSchema::GetCurrentVisualizationCacheID() const
{
	return CurrentCacheRefreshID;
}

void UEdDataGraphSchema::ForceVisualizationCacheClear() const
{
	CurrentCacheRefreshID++;
}

#undef LOCTEXT_NAMESPACE