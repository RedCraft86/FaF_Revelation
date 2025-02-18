// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameSection/GameSectionGraph.h"

UGameSectionGraph::UGameSectionGraph()
{
	CompatibleNode = UGameSectionNode::StaticClass();
	CompatibleEdge = nullptr;
}

UGameSectionNode::UGameSectionNode() : WidgetDelay(0.25f)
{
	Name = NSLOCTEXT("Toro", "NewSectionName", "New Section");
	Description = NSLOCTEXT("Toro", "NewSectionDesc", "A node containing data relevant to a section of the game.");
#if WITH_EDITORONLY_DATA
	ContextMenuName = INVTEXT("Game Section");
#endif
}

TSet<TSoftObjectPtr<UWorld>> UGameSectionNode::GetLevels() const
{
	TArray<TSoftObjectPtr<UWorld>> AllLevels;
	Levels.GenerateKeyArray(AllLevels);
	AllLevels.Insert(MainLevel, 0);
	AllLevels.RemoveAll([](const TSoftObjectPtr<UWorld>& Element)
	{
		return Element.IsNull();
	});
	
	return TSet(AllLevels);
}
#if WITH_EDITOR
void UGameSectionNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!MainLevel.IsNull()) Levels.Remove(MainLevel);
}
#endif