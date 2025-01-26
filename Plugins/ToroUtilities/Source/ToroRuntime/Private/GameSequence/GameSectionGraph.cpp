// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/GameSectionGraph.h"

UGameSectionGraph::UGameSectionGraph()
{
	CompatibleNode = UGameSectionNode::StaticClass();
	CompatibleEdge = nullptr;
}

UGameSectionNode::UGameSectionNode()
{
	Name = NSLOCTEXT("Toro", "NewSectionName", "New Section");
	Description = NSLOCTEXT("Toro", "NewSectionDesc", "A node containing data relevant to a section of the game.");
#if WITH_EDITORONLY_DATA
	ContextMenuName = INVTEXT("Game Section");
#endif
}
