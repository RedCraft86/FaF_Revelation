// Copyright (C) RedCraft86. All Rights Reserved.

#if 0
#include "GameSection/GameSectionGraph.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Background, "Background");

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
#endif