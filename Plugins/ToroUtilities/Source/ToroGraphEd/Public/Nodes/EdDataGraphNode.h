// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataNodeBase.h"
#include "EdGraph/EdGraphNode.h"
#include "EdDataGraphNode.generated.h"

class UEdDataGraph;
class SDataGraphNode;

UCLASS()
class UEdDataGraphNode final : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UEdDataGraphNode();

	// Potentially dangerous but it works, and it's editor only, so it's a problem for future me
	SDataGraphNode* SNode;

	UPROPERTY(VisibleAnywhere, Instanced, Category = DataGraph)
		TObjectPtr<UDataNodeBase> GraphNode;

	FText GetTextID() const;
	FText GetDescription() const;
	FVector2D GetNodeSize() const;
	UEdDataGraph* GetEdDataGraph() const;
	FLinearColor GetBackgroundColor() const;
	UEdGraphPin* GetInputPin() const { return Pins[0]; }
	UEdGraphPin* GetOutputPin() const { return Pins[1]; }
	void SetDataGraphNode(UDataNodeBase* InNode) { GraphNode = InNode; }
	
	virtual void PrepareForCopying() override;
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
