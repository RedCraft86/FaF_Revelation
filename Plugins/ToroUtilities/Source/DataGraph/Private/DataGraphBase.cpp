// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphBase.h"
#include "DataEdgeBase.h"
#include "DataNodeBase.h"

UDataGraphBase::UDataGraphBase() : GraphID(FGuid::NewGuid()), bCanBeCyclical(false)
	, CompatibleNode(UDataNodeBase::StaticClass()), CompatibleEdge(UDataEdgeBase::StaticClass())
{}

int32 UDataGraphBase::GetNodeDepth() const
{
	if (RootNodes.IsEmpty()) return 0;

	int32 Depth = 0;
	TArray<TObjectPtr<UDataNodeBase>> ThisLayer;
	TArray<TObjectPtr<UDataNodeBase>> LastLayer = RootNodes;
	while (!LastLayer.IsEmpty())
	{
		for (const TObjectPtr<UDataNodeBase>& Node : LastLayer)
		{
			ThisLayer.Append(Node->ChildNodes);
		}

		LastLayer = ThisLayer;
		ThisLayer.Reset();
		Depth++;
	}
	
	return Depth;
}

TArray<uint8> UDataGraphBase::ValidateSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	if (RootNodes.IsEmpty()) return {};
	if (InSequence.IsEmpty()) return bRootFallback ? TArray<uint8>{0} : TArray<uint8>();
	
	TArray<uint8> Sequence;
	TObjectPtr<UDataNodeBase> LastNode = nullptr;
	TArray<TObjectPtr<UDataNodeBase>> LastLayer = RootNodes;
	for (const uint8 Idx : InSequence)
	{
		if (LastLayer.IsValidIndex(Idx))
		{
			LastNode = LastLayer[Idx];
		}
		else break;

		if (LastNode)
		{
			LastLayer = LastNode->ChildNodes;
			Sequence.Add(Idx);
		}
		else break;
	}

	return Sequence.IsEmpty() && bRootFallback ? TArray<uint8>{0} : Sequence;
}

TArray<UDataNodeBase*> UDataGraphBase::GetNodesInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	if (RootNodes.IsEmpty()) return {};
	if (InSequence.IsEmpty())
	{
		return bRootFallback
			? TArray<UDataNodeBase*>{RootNodes[0]}
			: TArray<UDataNodeBase*>();
	}
	
	TArray<UDataNodeBase*> Nodes;
	TObjectPtr<UDataNodeBase> LastNode = nullptr;
	TArray<TObjectPtr<UDataNodeBase>> LastLayer = RootNodes;
	for (const uint8 Idx : InSequence)
	{
		if (LastLayer.IsValidIndex(Idx))
		{
			LastNode = LastLayer[Idx];
		}
		else break;

		if (LastNode)
		{
			LastLayer = LastNode->ChildNodes;
			Nodes.Add(LastNode);
		}
		else break;
	}

	return Nodes.IsEmpty() && bRootFallback ? TArray<UDataNodeBase*>{ RootNodes[0] } : Nodes;
}

UDataNodeBase* UDataGraphBase::GetLeafInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	const TArray<UDataNodeBase*> Nodes = GetNodesInSequence(InSequence, bRootFallback);
	return Nodes.IsEmpty() ? nullptr : Nodes.Last();
}

TArray<uint8> UDataGraphBase::GetSequenceFromNode(UDataNodeBase* InNode) const
{
	if (!InNode || InNode->OwningGraph != this) return {};
	if (InNode->IsRootNode()) return { (uint8)RootNodes.Find(InNode) };
	
	TArray<uint8> Sequence;
	TObjectPtr<UDataNodeBase> LastParent = InNode->ParentNodes[0];
	while (LastParent)
	{
		Sequence.Insert(LastParent->ChildNodes.Find(InNode), 0);
		LastParent = LastParent->IsRootNode() ? nullptr : LastParent->ParentNodes[0];
	}

	return Sequence;
}

#if WITH_EDITOR
void UDataGraphBase::ClearGraph()
{
	for (UDataNodeBase* Node : AllNodes)
	{
		if (!Node) continue;
		Node->ParentNodes.Empty();
		Node->ChildNodes.Empty();
		Node->Edges.Empty();
	}

	AllNodes.Empty();
	RootNodes.Empty();
}

FText UDataGraphBase::GetDescription() const
{
	return FText::Format(INVTEXT("{0} Nodes | {1} Roots{2}"),
		AllNodes.Num(), RootNodes.Num(), bCanBeCyclical
		? FText::GetEmpty() : INVTEXT(" | Cyclical"));
}
#endif
