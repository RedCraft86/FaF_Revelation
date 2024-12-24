// Copyright (C) RedCraft86. All Rights Reserved.

#include "AutoLayoutStrategy.h"
#include "DataGraphEdNode.h"
#include "DataGraphEdGraph.h"

#include "DataNodeBase.h"
#include "SDataGraphNode.h"
#include "Kismet/KismetMathLibrary.h"

UAutoLayoutStrategy::UAutoLayoutStrategy()
{
	Graph = nullptr;
	EdGraph = nullptr;
	MaxIteration = 50;
	OptimalDistance = 150;
}

FBox2D UAutoLayoutStrategy::GetNodeBound(UEdGraphNode* EdNode) const
{
	const int32 NodeWidth = GetNodeWidth(Cast<UDataGraphEdNode>(EdNode));
	const int32 NodeHeight = GetNodeHeight(Cast<UDataGraphEdNode>(EdNode));
	const FVector2D Max(EdNode->NodePosX + NodeWidth, EdNode->NodePosY + NodeHeight);
	const FVector2D Min(EdNode->NodePosX, EdNode->NodePosY);
	return FBox2D(Min, Max);
}

int32 UAutoLayoutStrategy::GetNodeWidth(const UDataGraphEdNode* EdNode) const
{
	return EdNode->SNode->GetCachedGeometry().GetLocalSize().X;
}

int32 UAutoLayoutStrategy::GetNodeHeight(const UDataGraphEdNode* EdNode) const
{
	return EdNode->SNode->GetCachedGeometry().GetLocalSize().Y;
}

FBox2D UAutoLayoutStrategy::GetActualBounds(UDataNodeBase* RootNode) const
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

void UAutoLayoutStrategy::RandomLayoutOneTree(UDataNodeBase* RootNode, const FBox2D& Bound)
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

			UDataGraphEdNode* EdNode_Node = EdGraph->NodeMap[Node];

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
