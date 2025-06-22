// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SGraphNode.h"

class SDataGraphNode final : public SGraphNode
{
public:

	SLATE_BEGIN_ARGS(SDataGraphNode){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UEdDataGraphNode* InNode);
	
	FText GetDescription() const;
	FText GetIdentifierText() const;
	FSlateColor GetBackgroundColor() const;
	void OnNameCommitted(const FText& InText, ETextCommit::Type CommitInfo);
	
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual void CreatePinWidgets() override;
	virtual void UpdateGraphNode() override;
};
