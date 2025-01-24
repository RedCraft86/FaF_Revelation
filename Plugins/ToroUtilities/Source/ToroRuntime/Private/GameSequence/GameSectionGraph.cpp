// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/GameSectionGraph.h"

UGameSectionGraph::UGameSectionGraph()
{
	CompatibleNode = UGameSectionNode::StaticClass();
	CompatibleEdge = nullptr;

	LoadingFacts = {
		INVTEXT("Endings: This game has multiple endings but only one of them is canonical to the game's lore."),
		INVTEXT("This game is a spiritual successor to my cancelled \"Fazbear Entertainment\" series."),
		INVTEXT("This iteration of the game is the 6th remake if you consider all the story rewrites, map redesigns, and game recodes.")
	};
}

UGameSectionNode::UGameSectionNode()
{
	Name = INVTEXT("New Section");
	Description = INVTEXT("A node containing data relevant to a section of the game.");
}
