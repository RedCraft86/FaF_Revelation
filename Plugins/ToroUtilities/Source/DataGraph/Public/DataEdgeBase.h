// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "DataEdgeBase.generated.h"

class UDataNodeBase;
class UDataGraphBase;

UCLASS(NotBlueprintable, BlueprintType)
class DATAGRAPH_API UDataEdgeBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UDataEdgeBase() : EdgeID(FGuid::NewGuid()), Name(NSLOCTEXT("ToroUtilities", "GenericEdgeName", "Generic Data Edge"))
		, bShouldDrawTitle(false), EdgeColor(0.9f, 0.9f, 0.9f, 1.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Edge)
		FGuid EdgeID;

	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		FText Name;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay, DisplayName = "Show Title")
		bool bShouldDrawTitle;

	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		FLinearColor EdgeColor;
#endif
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Edge, AdvancedDisplay)
		TObjectPtr<UDataGraphBase> OwningGraph;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Edge, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TObjectPtr<UDataNodeBase> StartNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Edge, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TObjectPtr<UDataNodeBase> EndNode;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return Name; }
	virtual void SetNodeTitle(const FText& InTitle) { Name = InTitle; }
#endif
};
