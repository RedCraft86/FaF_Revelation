// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "DataNodeBase.generated.h"

class UDataEdgeBase;
class UDataGraphBase;

UCLASS(BlueprintType, meta = (IsBlueprintBase = false))
class DATAGRAPH_API UDataNodeBase : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UDataNodeBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Node)
		FGuid NodeID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Node)
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Node, meta = (MultiLine = true))
		FText Description;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Node, AdvancedDisplay)
		FLinearColor BackgroundColor;

	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		FText ContextMenuName;
	
	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		uint8 ParentLimit;
	
	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		uint8 ChildLimit;

	UPROPERTY(VisibleAnywhere, Category = Node, AdvancedDisplay)
		TSubclassOf<UDataGraphBase> CompatibleGraph;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay)
		TObjectPtr<UDataGraphBase> OwningGraph;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay)
		TArray<TObjectPtr<UDataNodeBase>> ParentNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay)
		TArray<TObjectPtr<UDataNodeBase>> ChildNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Node, AdvancedDisplay)
		TMap<TObjectPtr<UDataNodeBase>, TObjectPtr<UDataEdgeBase>> Edges;

	UFUNCTION(BlueprintPure, Category = "DataGraphs|Node")
		bool IsRootNode() const { return ParentNodes.IsEmpty(); }

	UFUNCTION(BlueprintPure, Category = "DataGraphs|Node")
		bool IsLeafNode() const { return ChildNodes.IsEmpty(); }

	UFUNCTION(BlueprintPure, Category = "DataGraphs|Node")
		UDataEdgeBase* GetEdge(UDataNodeBase* ChildNode) const;
	
#if WITH_EDITOR
	virtual FText GetNodeTitle() const;
	virtual FText GetNodeDescription() const;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual bool CanConnectFrom(UDataNodeBase* Other, uint8 NumParentNodes, FString* ErrorMsg = nullptr);
	virtual bool CanConnectTo(UDataNodeBase* Other, uint8 NumChildNodes, FString* ErrorMsg = nullptr);
#endif
};
