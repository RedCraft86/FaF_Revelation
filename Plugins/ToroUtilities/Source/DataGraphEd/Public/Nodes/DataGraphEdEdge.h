// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DataGraphEdEdge.generated.h"

class UDataEdgeBase;
class UDataGraphEdNode;

UCLASS()
class UDataGraphEdEdge final : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UDataGraphEdEdge();

	UPROPERTY()
		TObjectPtr<UEdGraph> Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category = DataGraph)
		TObjectPtr<UDataEdgeBase> GraphEdge;

	void SetEdge(UDataEdgeBase* Edge);
	void CreateConnections(UDataGraphEdNode* Start, UDataGraphEdNode* End);
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void PrepareForCopying() override;
	
	UDataGraphEdNode* GetStartNode();
	UDataGraphEdNode* GetEndNode();
	
	UEdGraphPin* GetInputPin() const { return Pins[0]; }
	UEdGraphPin* GetOutputPin() const { return Pins[1]; }
};
