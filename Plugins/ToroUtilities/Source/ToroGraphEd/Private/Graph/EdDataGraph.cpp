// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/EdDataGraph.h"
#include "Nodes/EdDataGraphNode.h"
#include "Nodes/EdDataGraphEdge.h"
#include "ToroGraphEd.h"

void UEdDataGraph::RebuildDataGraph()
{
	UDataGraphBase* Graph = GetGraph();

	Clear();
	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdDataGraphNode* EdNode = Cast<UEdDataGraphNode>(Nodes[i]))
		{
			if (EdNode->GraphNode == nullptr) continue;
			UDataNodeBase* DataGraphNode = EdNode->GraphNode;
			NodeMap.Add(DataGraphNode, EdNode);
			Graph->AllNodes.Add(DataGraphNode);
			
			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];
				if (Pin->Direction != EGPD_Output) continue;
				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UDataNodeBase* ChildNode = nullptr;
					if (const UEdDataGraphNode* EdNode_Child = Cast<UEdDataGraphNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->GraphNode;
					}
					else if (UEdDataGraphEdge* EdNode_Edge = Cast<UEdDataGraphEdge>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						if (const UEdDataGraphNode* Child = EdNode_Edge->GetEndNode())
						{
							ChildNode = Child->GraphNode;
						}
					}

					if (ChildNode)
					{
						DataGraphNode->ChildNodes.Add(ChildNode);
						ChildNode->ParentNodes.Add(DataGraphNode);
					}
					else
					{
						UE_LOG(LogDataGraphEditor, Error, TEXT("UDataGraphEdGraph::RebuildDataGraph can't find child node"));
					}
				}
			}
		}
		else if (UEdDataGraphEdge* EdgeNode = Cast<UEdDataGraphEdge>(Nodes[i]))
		{
			const UEdDataGraphNode* StartNode = EdgeNode->GetStartNode();
			const UEdDataGraphNode* EndNode = EdgeNode->GetEndNode();
			UDataEdgeBase* Edge = EdgeNode->GraphEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				UE_LOG(LogDataGraphEditor, Error, TEXT("UDataGraphEdGraph::RebuildDataGraph add edge failed."));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->OwningGraph = Graph;
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->GraphNode;
			Edge->EndNode = EndNode->GraphNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (int i = 0; i < Graph->AllNodes.Num(); ++i)
	{
		UDataNodeBase* Node = Graph->AllNodes[i];
		if (Node->ParentNodes.IsEmpty())
		{
			Graph->RootNodes.Add(Node);
			SortNodes(Node);
		}

		Node->OwningGraph = Graph;
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}

	Graph->RootNodes.Sort([&](const UDataNodeBase& L, const UDataNodeBase& R)
	{
		const UEdDataGraphNode* EdNode_LNode = NodeMap[&L];
		const UEdDataGraphNode* EdNode_RNode = NodeMap[&R];
		return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
	});
}

UDataGraphBase* UEdDataGraph::GetGraph() const
{
	return CastChecked<UDataGraphBase>(GetOuter());
}

bool UEdDataGraph::Modify(bool bAlwaysMarkDirty)
{
	const bool bModified = Super::Modify(bAlwaysMarkDirty);
	
	GetGraph()->Modify();
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return bModified;
}

void UEdDataGraph::PostEditUndo()
{
	Super::PostEditUndo();
	NotifyGraphChanged();
}

void UEdDataGraph::Clear()
{
	UDataGraphBase* Graph = GetGraph();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (const UEdDataGraphNode* EdNode = Cast<UEdDataGraphNode>(Nodes[i]))
		{
			if (UDataNodeBase* DataGraphNode = EdNode->GraphNode)
			{
				DataGraphNode->ParentNodes.Reset();
				DataGraphNode->ChildNodes.Reset();
				DataGraphNode->Edges.Reset();
			}
		}
	}
}

void UEdDataGraph::SortNodes(UDataNodeBase* RootNode)
{
	int Level = 0;
	TArray<UDataNodeBase*> CurrLevelNodes = { RootNode };
	TArray<UDataNodeBase*> NextLevelNodes;
	TSet<UDataNodeBase*> Visited;

	while (!CurrLevelNodes.IsEmpty())
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UDataNodeBase* Node = CurrLevelNodes[i];
			Visited.Add(Node);

			auto Comp = [&](const UDataNodeBase& L, const UDataNodeBase& R)
			{
				const UEdDataGraphNode* EdNode_LNode = NodeMap[&L];
				const UEdDataGraphNode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			Node->ChildNodes.Sort(Comp);
			Node->ParentNodes.Sort(Comp);

			for (int j = 0; j < Node->ChildNodes.Num(); ++j)
			{
				if(!Visited.Contains(Node->ChildNodes[j]))
				{
					NextLevelNodes.Add(Node->ChildNodes[j]);
				}
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		Level++;
	}
}
