// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/NarrativeWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Narrative/ToroNarrative.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UNarrativeWidget::SetQuestsHidden(const bool bInHidden)
{
	if (bHideQuests != bInHidden && !IsAnimationPlaying(QuestFadeAnim))
	{
		bHideQuests = bInHidden;
		bHideQuests ? PlayAnimationForward(QuestFadeAnim) : PlayAnimationReverse(QuestFadeAnim);
	}
}

void UNarrativeWidget::QuestUpdatedNotify() const
{
	if (bNotifyObjectives && OnObjectiveUpdate.IsBound())
	{
		OnObjectiveUpdate.Execute();
	}
}

void UNarrativeWidget::RefreshQuestList(const UQuest* Quest, const UQuestBranch* Branch)
{
	QuestBranchBox->ClearChildren();
	if (Branch && (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure))
	{
		if (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
		{
			UQuestBranchWidget* BranchWidget = CreateWidget<UQuestBranchWidget>(this, BranchWidgetClass);
			BranchWidget->InitWidget(this, Branch);

			QuestBranchBox->AddChild(BranchWidget);
		}

		bHideQuests = false;
		QuestUpdatedNotify();
		PlayAnimation(QuestFadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UNarrativeWidget::OnQuestNewState(UQuest* Quest, const UQuestState* NewState)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewState]()
	{
		if (NewState && !NewState->Branches.IsEmpty())
		{
			QuestBranchBox->ClearChildren();
			for (const UQuestBranch* Branch : NewState->Branches)
			{
				if (Branch && !Branch->bHidden)
				{
					UQuestBranchWidget* BranchWidget = CreateWidget<UQuestBranchWidget>(this, BranchWidgetClass);
					BranchWidget->InitWidget(this, Branch);

					QuestBranchBox->AddChild(BranchWidget);
				}
			}

			bHideQuests = false;
			QuestUpdatedNotify();
			PlayAnimation(QuestFadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		}
	});
}

void UNarrativeWidget::OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch)
{
	if (Branch && Branch->DestinationState && Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
	{
		RefreshQuestList(Quest, Branch);
	}
}

void UNarrativeWidget::OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask, const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress)
{
	if (Branch && ProgressedTask && !ProgressedTask->IsComplete() && (!Branch->DestinationState
		|| Branch->DestinationState->StateNodeType != EStateNodeType::Failure))
	{
		RefreshQuestList(Quest, Branch);
	}
}

void UNarrativeWidget::OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this](){
		SetQuestsHidden(true);
	}, 0.6f, false);
}

void UNarrativeWidget::SelectDialogueReply(UDialogueNode_Player* Reply)
{
	NarrativeComponent->SelectDialogueOption(Reply);
	for (const TPair<FName, TObjectPtr<UDialogueOptionWidget>>& DialogueReply : DialogueReplies)
	{
		if (!DialogueReply.Value) continue;
		DialogueReply.Value->SetColorAndOpacity(FLinearColor::Gray);
		DialogueReply.Value->DisplayText->SetColorAndOpacity(FLinearColor::White);
		DialogueReply.Value->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (DialogueReply.Key == Reply->GetID())
		{
			DialogueReply.Value->MarkSelected();
		}
	}

	SkipLineButton->SetIsEnabled(true);
	SkipLineButton->SetRenderOpacity(1.0f);
	PlayAnimation(RepliesFadeAnim);
}

void UNarrativeWidget::OnDialogueBegan(UDialogue* Dialogue)
{
	SetQuestsHidden(true);
}

void UNarrativeWidget::OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	DialogueReplyBox->ClearChildren();
	PlayAnimationReverse(DialogueFadeAnim);
	if (GetAnimationCurrentTime(RepliesFadeAnim) > 0.0f)
	{
		PlayAnimationReverse(RepliesFadeAnim);
	}
}

void UNarrativeWidget::OnDialogueRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies)
{
	DialogueReplyBox->ClearChildren();
	if (PlayerReplies.Num() == 1)
	{
		NarrativeComponent->SelectDialogueOption(PlayerReplies[0]);
		return;
	}

	SkipLineButton->SetIsEnabled(false);
	SkipLineButton->SetRenderOpacity(0.5f);
	for (UDialogueNode_Player* Reply : PlayerReplies)
	{
		if (!Reply) continue;
		UDialogueOptionWidget* Widget = CreateWidget<UDialogueOptionWidget>(this, ReplyWidgetClass);
		Widget->SetPadding(DialogueReplyPadding);
		Widget->InitWidget(this, Dialogue, Reply);

		DialogueReplies.Add(Reply->GetID(), Widget);
		DialogueReplyBox->AddChild(Widget);
	}
	
	PlayAnimation(RepliesFadeAnim);
}

void UNarrativeWidget::OnDialoguePlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& DialogueLine)
{
	DialogueNameText->SetText(INVTEXT("You"));
	DialogueNameText->SetColorAndOpacity(DialoguePlayerColor);
	DialogueTitleText->SetVisibility(ESlateVisibility::Collapsed);
	DialogueLineText->SetText(InjectTextVariables(DialogueLine.Text));
	PlayAnimation(DialogueFadeAnim);
}

void UNarrativeWidget::OnDialoguePlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& DialogueLine)
{
	PlayAnimationReverse(RepliesFadeAnim);
}

void UNarrativeWidget::OnDialogueNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker)
{
	DialogueNameText->SetText(FText::FromName(Speaker.SpeakerID));
	DialogueNameText->SetColorAndOpacity(DialogueColor);

	DialogueTitleText->SetText(Speaker.SpeakerName);
	DialogueTitleText->SetVisibility(Speaker.SpeakerName.IsEmptyOrWhitespace()
		? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	DialogueLineText->SetText(InjectTextVariables(DialogueLine.Text));
	PlayAnimation(DialogueFadeAnim);
}

void UNarrativeWidget::OnLineSkipClicked()
{
	NarrativeComponent->CurrentDialogue->SkipCurrentLine();
}

void UNarrativeWidget::InitWidget()
{
	Super::InitWidget();
	NarrativeComponent = UToroNarrativeComponent::Get(this);

	NarrativeComponent->OnQuestNewState.AddDynamic(this, &ThisClass::OnQuestNewState);
	NarrativeComponent->OnQuestTaskCompleted.AddDynamic(this, &ThisClass::OnQuestTaskCompleted);
	NarrativeComponent->OnQuestTaskProgressChanged.AddDynamic(this, &ThisClass::OnQuestTaskProgressChanged);
	NarrativeComponent->OnQuestSucceeded.AddDynamic(this, &ThisClass::OnQuestSucceeded);

	NarrativeComponent->OnDialogueBegan.AddDynamic(this, &ThisClass::OnDialogueBegan);
	NarrativeComponent->OnDialogueFinished.AddDynamic(this, &ThisClass::OnDialogueFinished);
	NarrativeComponent->OnDialogueRepliesAvailable.AddDynamic(this, &ThisClass::OnDialogueRepliesAvailable);
	NarrativeComponent->OnPlayerDialogueLineStarted.AddDynamic(this, &ThisClass::OnDialoguePlayerLineStarted);
	NarrativeComponent->OnPlayerDialogueLineFinished.AddDynamic(this, &ThisClass::OnDialoguePlayerLineFinished);
	NarrativeComponent->OnNPCDialogueLineStarted.AddDynamic(this, &ThisClass::OnDialogueNPCLineStarted);

	SkipLineButton->OnClicked.AddDynamic(this, &ThisClass::OnLineSkipClicked);
}