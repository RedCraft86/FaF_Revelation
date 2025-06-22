// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MiscObjects/ToroDataAsset.h"
#include "DataGraphBase.generated.h"

class UDataEdgeBase;
class UDataNodeBase;

UCLASS(NotBlueprintable, BlueprintType)
class DATAGRAPH_API UDataGraphBase : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UDataGraphBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Graph)
		FGuid GraphID;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = Graph, AdvancedDisplay)
		bool bCanBeCyclical;
	
	UPROPERTY(VisibleAnywhere, Category = Graph, AdvancedDisplay)
		TSubclassOf<UDataNodeBase> CompatibleNode;

	UPROPERTY(VisibleAnywhere, Category = Graph, AdvancedDisplay)
		TSubclassOf<UDataEdgeBase> CompatibleEdge;

	UPROPERTY() TObjectPtr<UEdGraph> EdGraph;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Graph, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TArray<TObjectPtr<UDataNodeBase>> RootNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Graph, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TArray<TObjectPtr<UDataNodeBase>> AllNodes;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		int32 GetNodeDepth() const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		TArray<uint8> ValidateSequence(const TArray<uint8>& InSequence, const bool bRootFallback = true) const;
	
	UFUNCTION(BlueprintPure, Category = DataGraphs)
		TArray<UDataNodeBase*> GetNodesInSequence(const TArray<uint8>& InSequence, const bool bRootFallback = true) const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		UDataNodeBase* GetLeafInSequence(const TArray<uint8>& InSequence, const bool bRootFallback = true) const;

	UFUNCTION(BlueprintPure, Category = DataGraphs)
		TArray<uint8> GetSequenceFromNode(UDataNodeBase* InNode) const;

#if WITH_EDITOR
	void ClearGraph();
	virtual FText GetDescription() const override;
#endif
	
	template<typename T>
	TArray<T*> GetNodesInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
	{
		TArray<UDataNodeBase*> Nodes = GetNodesInSequence(InSequence, bRootFallback);
		TArray<TArray<T*>> OutNodes; OutNodes.Reserve(Nodes.Num());
		for (const UDataNodeBase* Node : Nodes)
		{
			OutNodes.Add(Cast<T>(Node));
		}

		return OutNodes;
	}
	
	template<typename T>
	T* GetLeafInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
	{
		return Cast<T>(GetLeafInSequence(InSequence, bRootFallback));
	}
};
