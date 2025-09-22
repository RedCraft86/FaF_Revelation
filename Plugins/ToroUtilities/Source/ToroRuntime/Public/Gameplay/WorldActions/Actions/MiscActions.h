// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Gameplay/WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Misc", meta = (Hidden))
struct TORORUNTIME_API FWATask_Misc : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Level Streaming")
struct TORORUNTIME_API FWTMisc_LevelStreaming final : public FWATask_Misc
{
	GENERATED_BODY()

	/* (Level, MakeVisibleAfterLoad) */
	UPROPERTY(EditAnywhere, Category = Action)
		TMap<TSoftObjectPtr<UWorld>, bool> Load;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<UWorld>> Unload;

	FWTMisc_LevelStreaming() {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};
