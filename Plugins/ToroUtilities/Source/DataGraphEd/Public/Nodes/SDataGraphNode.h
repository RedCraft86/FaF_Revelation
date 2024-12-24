// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SGraphNode.h"

class SDataGraphNode final : public SGraphNode
{
public:
	
	SLATE_BEGIN_ARGS(SDataGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UDataGraphEdNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	
	FText GetDescription() const;
	FSlateColor GetBackgroundColor() const;
	void OnNameCommitted(const FText& InText, ETextCommit::Type CommitInfo);
};
