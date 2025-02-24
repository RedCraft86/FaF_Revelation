﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "DataNodeBase.h"
#include "DataGraphBase.h"
#include "MetasoundSource.h"
#include "GameSection/UDSTypes.h"
#include "Misc/ToroSequenceActor.h"
#include "Misc/TeleportTargetActor.h"
#include "Inventory/InventoryComponent.h"
#include "GameSectionGraph.generated.h"

UCLASS()
class TORORUNTIME_API UGameSectionGraph final : public UDataGraphBase
{
	GENERATED_BODY()

public:
	
	UGameSectionGraph();
};

UCLASS()
class TORORUNTIME_API UGameSectionNode final : public UDataNodeBase
{
	GENERATED_BODY()

public:
	
	UGameSectionNode();
	
	UPROPERTY(EditAnywhere, Category = Level)
		float WidgetDelay;
	
	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<ATeleportTargetActor> Teleport;
	
	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(EditAnywhere, Category = Level)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

	UPROPERTY(EditAnywhere, Category = "Level|Sequences")
		TSoftObjectPtr<AToroSequenceActor> StartSequence;

	UPROPERTY(EditAnywhere, Category = "Level|Sequences")
		TSoftObjectPtr<AToroSequenceActor> WaitSequence;

	UPROPERTY(EditAnywhere, Category = "Level|Sequences")
		TSoftObjectPtr<AToroSequenceActor> EndSequence;

	UPROPERTY(EditAnywhere, Category = Game, meta = (Categories = "Content"))
		TSet<FGameplayTag> UnlockContent;

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftClassPtr<UQuest> Quest;

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftObjectPtr<UMetaSoundSource> Theme;

	UPROPERTY(EditAnywhere, Category = Game, DisplayName = "Sky & Weather")
		FUDSSettings SkyWeather;
	
	UPROPERTY(EditAnywhere, Category = Player)
		TArray<FInventorySlotData> Inventory;

	bool PlayStart() const
	{
		if (StartSequence.LoadSynchronous()) StartSequence->Play();
		return StartSequence.IsValid();
	}
	bool PlayEnd() const
	{
		if (EndSequence.LoadSynchronous()) EndSequence->Play();
		return EndSequence.IsValid();
	}
	bool PlayWait() const
	{
		if (WaitSequence.LoadSynchronous()) WaitSequence->Play();
		return WaitSequence.IsValid();
	}
	bool StopWait() const
	{
		if (WaitSequence.LoadSynchronous()) WaitSequence->Stop();
		return WaitSequence.IsValid();
	}
	
	TSet<TSoftObjectPtr<UWorld>> GetLevels() const;

#if WITH_EDITOR
private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
