// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DataGraphEdNode.generated.h"

class UDataNodeBase;
class UDataGraphEdGraph;
class SDataGraphNode;

UCLASS()
class UDataGraphEdNode final : public UEdGraphNode
{
	GENERATED_BODY()

public:
	
	UDataGraphEdNode();

	SDataGraphNode* SNode;
	
	UPROPERTY(VisibleAnywhere, Instanced, Category = DataGraph)
		TObjectPtr<UDataNodeBase> GraphNode;

	void SetDataGraphNode(UDataNodeBase* InNode);
	UDataGraphEdGraph* GetDataGraphEdGraph() const;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	FText GetDescription() const;
	FLinearColor GetBackgroundColor() const;
	UEdGraphPin* GetInputPin() const { return Pins[0]; }
	UEdGraphPin* GetOutputPin() const { return Pins[1]; }
};
