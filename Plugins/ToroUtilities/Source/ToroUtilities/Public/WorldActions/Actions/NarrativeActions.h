// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "Dialogue.h"
#include "NarrativeDataTask.h"
#include "WorldActions/WorldActionBase.h"
#include "NarrativeActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Narrative Actions", meta = (Hidden))
struct TOROUTILITIES_API FWTaskNarrativeActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Start Dialogue")
struct TOROUTILITIES_API FWTaskStartDialogue final : public FWTaskNarrativeActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Dialogue)
		TSubclassOf<UDialogue> DialogueClass;

	FWTaskStartDialogue() {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Start Quest")
struct TOROUTILITIES_API FWTaskStartQuest final : public FWTaskNarrativeActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Quest)
		TSubclassOf<UQuest> QuestClass;

	FWTaskStartQuest() {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Complete Task")
struct TOROUTILITIES_API FWTaskCompleteTask final : public FWTaskNarrativeActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Task)
		TObjectPtr<UNarrativeDataTask> TaskObject;

	UPROPERTY(EditAnywhere, Category = Task)
		FString Argument;

	UPROPERTY(EditAnywhere, Category = Task, meta = (ClampMin = 1, UIMin = 1))
		uint8 Quantity;

	FWTaskCompleteTask() : Quantity(1)
	{
		TaskObject = LoadObject<UNarrativeDataTask>(nullptr,
			TEXT("/Narrative/DefaultTasks/LooseTask.LooseTask"));
	}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};