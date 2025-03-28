﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataNodeBase.h"
#include "DataGraphBase.h"

UDataNodeBase::UDataNodeBase() : NodeID(FGuid::NewGuid()), BackgroundColor(0.1f, 0.1f, 0.1f)
	, ContextMenuName(NSLOCTEXT("ToroUtilities", "GenericNodeName", "Generic Data Node")), ParentLimit(255), ChildLimit(255)
	, CompatibleGraph(UDataGraphBase::StaticClass())
{}

UDataEdgeBase* UDataNodeBase::GetEdge(UDataNodeBase* ChildNode) const
{
	TObjectPtr<UDataEdgeBase> const* FoundEdge = Edges.Find(ChildNode);
	return FoundEdge ? *FoundEdge : nullptr;
}

FText UDataNodeBase::GetNodeTitle() const
{
	return Name.IsEmptyOrWhitespace() ? NSLOCTEXT("ToroUtilities", "GenericNodeName", "Generic Data Node") : Name;
}

FText UDataNodeBase::GetNodeDescription() const
{
	return Description;
}

void UDataNodeBase::SetNodeTitle(const FText& NewTitle)
{
	Name = NewTitle;
}

#if WITH_EDITOR
bool UDataNodeBase::CanConnectFrom(UDataNodeBase* Other, const uint8 NumParentNodes, FString* ErrorMsg)
{
	if (!Other)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Connecting data node is NULL.");
		return false;
	}
	
	if (NumParentNodes >= ParentLimit)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Parent node limit exceeded.");
		return false;
	}

	return true;
}

bool UDataNodeBase::CanConnectTo(UDataNodeBase* Other, const uint8 NumChildNodes, FString* ErrorMsg)
{
	if (!Other)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Connecting data node is NULL.");
		return false;
	}
	
	if (NumChildNodes >= ChildLimit)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Child node limit exceeded.");
		return false;
	}

	return true;
}
#endif
