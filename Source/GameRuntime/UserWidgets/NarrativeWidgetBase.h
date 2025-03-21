// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "Dialogue.h"
#include "ToroWidget.h"
#include "SubWidgets/NarrativeSubWidgets.h"
#include "Framework/ToroPlayerController.h"
#include "UserSettings/ToroUserSettings.h"
#include "NarrativeWidgetBase.generated.h"

class UButton;
class UTextBlock;

UCLASS(Abstract)
class GAMERUNTIME_API UNarrativeWidgetBase : public UToroWidget
{
	GENERATED_BODY()

	friend class UDialogueOptionWidgetBase;

public:

	UNarrativeWidgetBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), DialoguePlayerColor(FLinearColor::Yellow)
		, DialogueColor(FLinearColor::White), bNotifyObjectives(false), bHideQuests(false)
	{
		ZOrder = 56;
	}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> QuestBranchBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DialogueNameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DialogueTitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UExprTextBlock> DialogueLineText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SkipLineButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> DialogueReplyBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> QuestFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> DialogueFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> RepliesFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> HideFadeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor DialoguePlayerColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, DisplayName = "Dialogue NPC Color")
		FLinearColor DialogueColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FMargin DialogueReplyPadding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		bool bNotifyObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<UQuestBranchWidgetBase> BranchWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<UDialogueOptionWidgetBase> ReplyWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Settings)
		TObjectPtr<UToroNarrativeComponent> NarrativeComponent;

	UFUNCTION(BlueprintCallable, Category = NarrativeWidget)
		void SetQuestsHidden(const bool bInHidden);

	UFUNCTION(BlueprintPure, Category = NarrativeWidget)
		bool AreQuestsHidden() const { return bHideQuests; }

	UFUNCTION(BlueprintNativeEvent)
		FText InjectTextVariables(const FText& InText) const;
	FText InjectTextVariables_Implementation(const FText& InText) const
	{
		TMap<FString, FString> Args;
		Args.Add(TEXT("%Username%"), UToroUserSettings::Get()->GetUsername());

		FString Out = InText.ToString();
		for (const TPair<FString, FString>& Arg : Args) Out.ReplaceInline(*Arg.Key, *Arg.Value);
		
		return FText::FromString(Out);
	}
	
protected:

	UPROPERTY() bool bHideQuests;
	UPROPERTY() FGameInputModeData CachedInputMode;
	UPROPERTY(Transient) TObjectPtr<AToroPlayerCharacter> PlayerChar;
	UPROPERTY(Transient) TMap<FName, TObjectPtr<UDialogueOptionWidgetBase>> DialogueReplies;

	void QuestUpdatedNotify() const;
	void RefreshQuestList(const UQuest* Quest, const UQuestBranch* Branch);
	UFUNCTION() void OnQuestNewState(UQuest* Quest, const UQuestState* NewState);
	UFUNCTION() void OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch);
	UFUNCTION() void OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask, const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
	UFUNCTION() void OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage);

	void SelectDialogueReply(UDialogueNode_Player* Reply);
	UFUNCTION() void OnDialogueBegan(UDialogue* Dialogue);
	UFUNCTION() void OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue);
	UFUNCTION() void OnDialogueRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies);
	UFUNCTION() void OnDialoguePlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
	UFUNCTION() void OnDialoguePlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& DialogueLine);
	UFUNCTION() void OnDialogueNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker);
	UFUNCTION() void OnLineSkipClicked();

	virtual void InitWidget() override;
};