// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataGraphBase.h"
#include "EdGraph/EdGraph.h"
#include "EdDataGraph.generated.h"

UCLASS()
class UEdDataGraph final : public UEdGraph
{
	GENERATED_BODY()

public:

	UEdDataGraph() {}

	UPROPERTY(Transient)
		TMap<TObjectPtr<UDataNodeBase>, TObjectPtr<class UEdDataGraphNode>> NodeMap;

	UPROPERTY(Transient)
		TMap<TObjectPtr<UDataEdgeBase>, TObjectPtr<class UEdDataGraphEdge>> EdgeMap;

	void RebuildDataGraph();
	UDataGraphBase* GetGraph() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;
	
private:
	
	void Clear();
	void SortNodes(UDataNodeBase* RootNode);
};
