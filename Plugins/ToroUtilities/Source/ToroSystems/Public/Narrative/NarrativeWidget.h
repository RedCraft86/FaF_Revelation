// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "Dialogue.h"
#include "DialogueSM.h"
#include "ToroUtilities.h"
#include "NarrativeSubWidgets.h"
#include "UserWidgets/SubtitleWidget.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "NarrativeWidget.generated.h"

class UButton;
class UTextBlock;
class UToroNarrative;

UCLASS(Abstract)
class TOROSYSTEMS_API UQuestWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UQuestWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> BranchBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<UQuestBranchWidget> BranchWidgetClass;

	UFUNCTION(BlueprintImplementableEvent)
		FText InjectTextVariables(const FText& InText) const;

private:

	UPROPERTY(Transient) TObjectPtr<UToroNarrative> Narrative;

	void QuestUpdatedNotify() const;
	void RefreshQuestList(const UQuest* Quest, const UQuestBranch* Branch);

	UFUNCTION() void OnDialogueBegan(UDialogue* Dialogue);
	UFUNCTION() void OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue);

	UFUNCTION() void OnQuestNewState(UQuest* Quest, const UQuestState* NewState);
	UFUNCTION() void OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch);
	UFUNCTION() void OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask, const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
	UFUNCTION() void OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage);

	virtual void InitWidget() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};

UCLASS(Abstract)
class TOROSYSTEMS_API UDialogueWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SpeakerText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UExprTextBlock> LineText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SkipButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ReplyBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> RepliesAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<UDialogueOptionWidget> ReplyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor PlayerColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, DisplayName = "NPC Color")
		FLinearColor NPCColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FMargin ReplyPadding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TObjectPtr<const UNarrativeDataTask> ReplyTaskType;

	UFUNCTION(BlueprintImplementableEvent)
		FText InjectTextVariables(const FText& InText) const;
	
	bool HasChosenOption(const UDialogueNode_Player* InChoice) const;
	void SelectReplyOption(UDialogueNode_Player* InChoice);

private:

	UPROPERTY(Transient) TObjectPtr<UToroNarrative> Narrative;
	UPROPERTY(Transient) TObjectPtr<USubtitleWidget> SimpleSubtitles;
	UPROPERTY(Transient) TMap<FName, TObjectPtr<UDialogueOptionWidget>> Replies;

	UFUNCTION() void OnSkipClicked();
	UFUNCTION() void OnBegan(UDialogue* Dialogue);
	UFUNCTION() void OnFinished(UDialogue* Dialogue, const bool bStartingNewDialogue);
	UFUNCTION() void OnRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies);
	UFUNCTION() void OnPlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line);
	UFUNCTION() void OnPlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line);
	UFUNCTION() void OnNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker);

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
