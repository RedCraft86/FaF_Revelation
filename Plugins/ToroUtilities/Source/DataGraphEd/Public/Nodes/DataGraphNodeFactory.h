// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class FDataGraphNodeFactory final : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
