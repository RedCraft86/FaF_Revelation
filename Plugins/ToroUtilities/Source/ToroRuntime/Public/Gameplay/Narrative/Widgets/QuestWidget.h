// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QuestSM.h"
#include "Dialogue.h"
#include "ToroRuntime.h"
#include "Components/VerticalBox.h"
#include "UserInterface/ToroManagedWidget.h"
#include "QuestWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UQuestWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UQuestWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UVerticalBox> BranchContainer;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FText TaskPrefix;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FSlateFontInfo TaskFont;

	UPROPERTY(Transient)
		TMap<TObjectPtr<const UQuestBranch>, TObjectPtr<UVerticalBox>> BranchBoxes;

	TObjectPtr<class UNarrativeManager> Manager;

	UFUNCTION() void OnDialogueBegan(UDialogue* Dialogue);
	UFUNCTION() void OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue);

	UFUNCTION() void OnQuestNewState(UQuest* Quest, const UQuestState* NewState);
	UFUNCTION() void OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch);
	UFUNCTION() void OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask, const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
	UFUNCTION() void OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage);

	void ShowObjectiveNotice() const;
	void AddOrUpdateQuestBranch(const UQuestBranch* Branch);
	UVerticalBox* FindOrAddTaskContainer(const UQuestBranch* Branch);
	void ClearQuestContainers(const UQuest* Quest);

	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
