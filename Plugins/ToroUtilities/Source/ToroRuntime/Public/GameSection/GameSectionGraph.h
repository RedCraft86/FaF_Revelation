// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataNodeBase.h"
#include "DataGraphBase.h"
#include "GameplayTagContainer.h"
#include "GameSectionGraph.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FLoadingImageArray
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = LoadingImageArray)
		TSet<TSoftObjectPtr<UTexture2D>> Images;
};

UCLASS()
class TORORUNTIME_API UGameSectionGraph final : public UDataGraphBase
{
	GENERATED_BODY()

public:
	
	UGameSectionGraph();

	UPROPERTY(EditAnywhere, Category = Graph, meta = (Multiline = true))
		TArray<FText> LoadingFacts;

	UPROPERTY(EditAnywhere, Category = Graph, meta = (ForceInlineRow, Categories = "Backgrounds"))
		TMap<FGameplayTag, FLoadingImageArray> Backgrounds;
};

UCLASS()
class TORORUNTIME_API UGameSectionNode final : public UDataNodeBase
{
	GENERATED_BODY()

public:
	
	UGameSectionNode();
};