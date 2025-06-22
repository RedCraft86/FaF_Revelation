// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SGraphNode.h"

class SDataGraphEdge final : public SGraphNode
{
public:

	SLATE_BEGIN_ARGS(SDataGraphEdge){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UEdDataGraphEdge* InNode);

	void OnNameCommitted(const FText& InText, ETextCommit::Type CommitInfo);
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;

	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;
	virtual bool RequiresSecondPassLayout() const override { return true; }
	virtual void UpdateGraphNode() override;

private:
	
	FSlateColor GetEdgeColor() const;
	EVisibility GetEdgeImageVisibility() const;
	EVisibility GetEdgeTitleVisibility() const;

	TSharedPtr<STextEntryPopup> TextEntryWidget;
};
