// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "DataEdgeBase.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TOROGRAPH_API UDataEdgeBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UDataEdgeBase(): EdgeID(FGuid::NewGuid())
		, Name(NSLOCTEXT("ToroUtilities", "DataEdgeName", "Generic Data Edge"))
		, bShouldDrawTitle(false), EdgeColor(0.9f, 0.9f, 0.9f, 1.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Edge)
		FGuid EdgeID;

	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		FText Name;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		bool bShouldDrawTitle;

	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		FLinearColor EdgeColor;
#endif
	
	UPROPERTY(BlueprintReadOnly, Category = Edge, AdvancedDisplay)
		TObjectPtr<UDataNodeBase> StartNode;

	UPROPERTY(BlueprintReadOnly, Category = Edge, AdvancedDisplay)
		TObjectPtr<UDataNodeBase> EndNode;
	
	UPROPERTY() TObjectPtr<UDataGraphBase> OwningGraph;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return Name; }
	virtual void SetNodeTitle(const FText& InTitle) { Name = InTitle; }
#endif
};
