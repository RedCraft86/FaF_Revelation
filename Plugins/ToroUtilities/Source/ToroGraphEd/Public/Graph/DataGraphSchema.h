// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "DataGraphSchema.generated.h"

USTRUCT()
struct FDataGraphAction_NewNode final : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FDataGraphAction_NewNode(): NodeTemplate(nullptr) {}
	FDataGraphAction_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr)
	{}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	TObjectPtr<class UEdDataGraphNode> NodeTemplate;
};

USTRUCT()
struct FDataGraphAction_NewEdge final : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FDataGraphAction_NewEdge(): NodeTemplate(nullptr) {}
	FDataGraphAction_NewEdge(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr)
	{}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	TObjectPtr<class UEdDataGraphEdge> NodeTemplate;
};

UCLASS()
class UEdDataGraphSchema final : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	
	UEdDataGraphSchema() {}
	
	void GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin);
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
	virtual UEdGraphPin* DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const override;
	virtual bool SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;

private:
	
	static int32 CurrentCacheRefreshID;
};
