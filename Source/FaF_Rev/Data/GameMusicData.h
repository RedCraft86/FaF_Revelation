﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "FaF_Rev.h"
#include "EnemyData.h"
#include "Engine/DataAsset.h"
#include "PulldownStruct/PulldownStructBase.h"
#include "GameMusicData.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FAF_REV_API FGameMusicTypeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "GameMusic")
		TSoftObjectPtr<USoundBase> Music;

	UPROPERTY(EditAnywhere, Category = "GameMusic")
		float TransitionTime;

	UPROPERTY(EditAnywhere, Category = "GameMusic")
		bool bRestartOnTransition;

	FGameMusicTypeData() : Music(nullptr), TransitionTime(2.0f), bRestartOnTransition(false) {}
};

USTRUCT(BlueprintInternalUseOnly)
struct FAF_REV_API FGameMusicData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GameMusic", meta = (ReadOnlyKeys, TitleProperty = "Music"))
		TMap<EEnemyAIMode, FGameMusicTypeData> Tracks;

#if WITH_EDITOR
protected:
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		Super::OnDataTableChanged(InDataTable, InRowName);
		
		if (Tracks.Num() == 4) return;
		for (const EEnemyAIMode Mode : TEnumRange<EEnemyAIMode>())
		{
			if (!Tracks.Contains(Mode)) Tracks.Add(Mode);
		}
	}
#endif
};

USTRUCT(BlueprintType)
struct FAF_REV_API FGameMusicID : public FPulldownStructBase
{
	GENERATED_BODY()
	SETUP_PULLDOWN(FGameMusicID)
	FGameMusicData GetMusicData() const;
};
