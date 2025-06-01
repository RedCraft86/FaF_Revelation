// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "Dialogue.h"
#include "NarrativeDataTask.h"
#include "WorldActions/WorldActionBase.h"
#include "NarrativeActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Narrative", meta = (Hidden))
struct FAFREVGAME_API FWATaskNarrative : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Start Dialogue")
struct FAFREVGAME_API FWANarrativeDialogue final : public FWATaskNarrative
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Dialogue)
		TSubclassOf<UDialogue> DialogueClass;

	FWANarrativeDialogue() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Start Quest")
struct FAFREVGAME_API FWANarrativeQuest final : public FWATaskNarrative
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Quest)
		TSubclassOf<UQuest> QuestClass;

	FWANarrativeQuest() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Complete Task")
struct FAFREVGAME_API FWANarrativeTask final : public FWATaskNarrative
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Task)
		TObjectPtr<UNarrativeDataTask> TaskObject;

	UPROPERTY(EditAnywhere, Category = Task)
		FString Argument;

	UPROPERTY(EditAnywhere, Category = Task, meta = (ClampMin = 1, UIMin = 1))
		uint8 Quantity;

	FWANarrativeTask(): Quantity(1)
	{
		TaskObject = LoadObject<UNarrativeDataTask>(nullptr,
			TEXT("/Narrative/DefaultTasks/LooseTask.LooseTask"));
	}

protected:

	virtual void RunAction() override;
};