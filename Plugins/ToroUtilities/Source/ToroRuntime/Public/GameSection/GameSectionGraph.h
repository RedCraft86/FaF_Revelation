// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if 0
#include "DataNodeBase.h"
#include "DataGraphBase.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameSectionGraph.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Background);

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FLoadingImageSet
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = LoadingImageSet)
		TSet<TSoftObjectPtr<UTexture2D>> Images;
};

UCLASS()
class TORORUNTIME_API UGameSectionGraph final : public UDataGraphBase
{
	GENERATED_BODY()

public:
	
	UGameSectionGraph();

	UPROPERTY(EditAnywhere, Category = Graph, meta = (Multiline = true))
		TMap<FName, FText> LoadingFacts;

	UPROPERTY(EditAnywhere, Category = Graph, meta = (ForceInlineRow, Categories = "Backgrounds"))
		TMap<FGameplayTag, FLoadingImageSet> Backgrounds;
};

UCLASS()
class TORORUNTIME_API UGameSectionNode final : public UDataNodeBase
{
	GENERATED_BODY()

public:
	
	UGameSectionNode();
	
	UPROPERTY(EditAnywhere, Category = GameSection)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;
};
#endif