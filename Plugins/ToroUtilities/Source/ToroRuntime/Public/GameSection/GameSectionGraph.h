// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataNodeBase.h"
#include "DataGraphBase.h"
#include "Misc/ToroSequenceActor.h"
#include "Misc/TeleportTargetActor.h"
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
		TSoftObjectPtr<AToroSequenceActor> StartSequence;

	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<AToroSequenceActor> WaitSequence;

	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<AToroSequenceActor> EndSequence;
	
	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(EditAnywhere, Category = Level)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

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
};
