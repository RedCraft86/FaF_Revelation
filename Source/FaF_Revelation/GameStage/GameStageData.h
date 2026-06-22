// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "NativeGameplayTags.h"
#include "MiscActors/TeleportPoint.h"
#include "GameStageData.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameStage)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Objective)

UCLASS(NotBlueprintable, BlueprintType)
class UGameStageData final : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = StageData, meta = (Categories = "GameStage"))
		FGameplayTag StageEvent;

	UPROPERTY(EditAnywhere, Category = StageData)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

	UPROPERTY(EditAnywhere, Category = StageData, meta = (Categories = "Objective"))
		FGameplayTagContainer Objectives;

	UPROPERTY(EditAnywhere, Category = "StageData|Teleport")
		bool bContinuous;

	UPROPERTY(EditAnywhere, Category = "StageData|Teleport", DisplayName = "Target")
		TSoftObjectPtr<ATeleportPoint> Teleport;
};
