// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraph.h"
#include "DataGraphEdGraph.generated.h"

UCLASS()
class UDataGraphEdGraph final : public UEdGraph
{
	GENERATED_BODY()

public:
	
	UDataGraphEdGraph() {}

	UPROPERTY(Transient) TMap<TObjectPtr<class UDataNodeBase>, TObjectPtr<class UDataGraphEdNode>> NodeMap;
	UPROPERTY(Transient) TMap<TObjectPtr<class UDataEdgeBase>, TObjectPtr<class UDataGraphEdEdge>> EdgeMap;


	void RebuildDataGraph();
	class UDataGraphBase* GetGraph() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;
	
private:
	
	void Clear();
	void SortNodes(UDataNodeBase* RootNode);
};
