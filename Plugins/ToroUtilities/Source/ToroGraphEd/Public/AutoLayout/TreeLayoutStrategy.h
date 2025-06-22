// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AutoLayoutBase.h"
#include "TreeLayoutStrategy.generated.h"

UCLASS()
class UTreeLayoutStrategy final : public UAutoLayoutBase
{
	GENERATED_BODY()

public:

	UTreeLayoutStrategy() {}

	virtual void Layout(UEdGraph* InGraph) override;

private:
	
	bool ResolveConflictPass(UDataNodeBase* Node);
	void InitPass(UDataNodeBase* RootNode, const FVector2D& Anchor);
	bool ResolveConflict(UDataNodeBase* LRoot, UDataNodeBase* RRoot);
	void GetLeftContour(UDataNodeBase* RootNode, int32 Level, TArray<UEdDataGraphNode*>& Contour);
	void GetRightContour(UDataNodeBase* RootNode, int32 Level, TArray<UEdDataGraphNode*>& Contour);
	void ShiftSubTree(UDataNodeBase* RootNode, const FVector2D& Offset);
	void UpdateParentNodePosition(UDataNodeBase* RootNode) const;
};
