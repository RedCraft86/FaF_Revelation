// Copyright (C) RedCraft86. All Rights Reserved.

#include "AutoLayout/AutoLayoutBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nodes/EdDataGraphNode.h"
#include "Graph/EdDataGraph.h"
#include "DataNodeBase.h"

UAutoLayoutBase::UAutoLayoutBase(): MaxIteration(50), OptimalDistance(150)
{
	Graph = nullptr;
	EdGraph = nullptr;
}

FBox2D UAutoLayoutBase::GetNodeBound(UEdGraphNode* EdNode)
{
	const FVector2D NodeSize = Cast<UEdDataGraphNode>(EdNode)->GetNodeSize();
	const FVector2D Pos(EdNode->NodePosX, EdNode->NodePosY);
	return FBox2D(Pos, Pos + NodeSize);
}

FBox2D UAutoLayoutBase::GetActualBounds(UDataNodeBase* RootNode) const
{
	int Level = 0;
	TArray<UDataNodeBase*> CurrLevelNodes = { RootNode };
	TArray<UDataNodeBase*> NextLevelNodes;

	FBox2D Bounds = GetNodeBound(EdGraph->NodeMap[RootNode]);
	while (!CurrLevelNodes.IsEmpty())
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UDataNodeBase* Node = CurrLevelNodes[i];
			check(Node);

			Bounds += GetNodeBound(EdGraph->NodeMap[Node]);
			for (int j = 0; j < Node->ChildNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		Level++;
	}
	
	return Bounds;
}

void UAutoLayoutBase::RandomLayoutOneTree(UDataNodeBase* RootNode, const FBox2D& Bound)
{
	int Level = 0;
	TArray<UDataNodeBase*> CurrLevelNodes = { RootNode };
	TArray<UDataNodeBase*> NextLevelNodes;

	while (!CurrLevelNodes.IsEmpty())
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UDataNodeBase* Node = CurrLevelNodes[i];
			check(Node);

			UEdDataGraphNode* EdNode_Node = EdGraph->NodeMap[Node];

			EdNode_Node->NodePosX = UKismetMathLibrary::RandomFloatInRange(Bound.Min.X, Bound.Max.X);
			EdNode_Node->NodePosY = UKismetMathLibrary::RandomFloatInRange(Bound.Min.Y, Bound.Max.Y);

			for (int j = 0; j < Node->ChildNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		Level++;
	}
}
