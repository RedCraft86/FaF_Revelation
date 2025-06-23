// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToroDataEdge.generated.h"

class UToroDataNode;
class UToroDataGraph;

UCLASS(NotBlueprintable, BlueprintType)
class TOROGRAPH_API UToroDataEdge : public UDataAsset
{
	GENERATED_BODY()

public:

	UToroDataEdge(): EdgeID(FGuid::NewGuid()), Name(INVTEXT("Generic Data Edge"))
		, bDrawTitle(false), EdgeColor(0.9f, 0.9f, 0.9f, 1.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Edge)
		FGuid EdgeID;

	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		FText Name;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay, DisplayName = "Show Title")
		bool bDrawTitle;

	UPROPERTY(EditDefaultsOnly, Category = Edge, AdvancedDisplay)
		FLinearColor EdgeColor;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Edge, AdvancedDisplay)
		TObjectPtr<UToroDataGraph> OwningGraph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Edge, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TObjectPtr<UToroDataNode> StartNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Edge, AdvancedDisplay, meta = (DisplayThumbnail = false))
		TObjectPtr<UToroDataNode> EndNode;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return Name; }
	virtual void SetNodeTitle(const FText& InTitle) { Name = InTitle; }
#endif
};
