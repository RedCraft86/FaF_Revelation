// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AutoLayoutStrategy.h"
#include "TreeLayoutStrategy.generated.h"

UCLASS()
class UTreeLayoutStrategy final : public UAutoLayoutStrategy
{
	GENERATED_BODY()
	
public:
	
	UTreeLayoutStrategy() {}
	
	void Layout(UEdGraph* InGraph) override;

private:
	
	void InitPass(UDataNodeBase* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UDataNodeBase* Node);
	bool ResolveConflict(UDataNodeBase* LRoot, UDataNodeBase* RRoot);
	void GetLeftContour(UDataNodeBase* RootNode, int32 Level, TArray<UDataGraphEdNode*>& Contour);
	void GetRightContour(UDataNodeBase* RootNode, int32 Level, TArray<UDataGraphEdNode*>& Contour);
	void ShiftSubTree(UDataNodeBase* RootNode, const FVector2D& Offset);
	void UpdateParentNodePosition(UDataNodeBase* RootNode);
};
