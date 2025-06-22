// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataEdgeBase.h"
#include "EdGraph/EdGraphNode.h"
#include "EdDataGraphEdge.generated.h"

class UEdDataGraphNode;

UCLASS()
class UEdDataGraphEdge final : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UEdDataGraphEdge();

	UPROPERTY(Transient) TObjectPtr<UEdGraph> Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category = DataGraph)
		TObjectPtr<UDataEdgeBase> GraphEdge;

	UEdDataGraphNode* GetEndNode();
	UEdDataGraphNode* GetStartNode();
	UEdGraphPin* GetInputPin() const { return Pins[0]; }
	UEdGraphPin* GetOutputPin() const { return Pins[1]; }
	void SetEdge(UDataEdgeBase* Edge) { GraphEdge = Edge; }
	void CreateConnections(const UEdDataGraphNode* Start, const UEdDataGraphNode* End);

	virtual void PrepareForCopying() override;
	virtual void AllocateDefaultPins() override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	
};
